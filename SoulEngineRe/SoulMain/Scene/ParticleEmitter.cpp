#include "ParticleEmitter.h"
#include "SubMesh.h"

namespace Soul
{
	ParticleEmitter::ParticleEmitter()
		:
		rng(std::random_device()())
	{
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}

	bool ParticleEmitter::Initialize(SubMesh* particlesMesh)
	{
		mParticlesMesh = particlesMesh;
		bool result;
		result = InitializeParticleSystem();
		result = InitializeBuffer();
		return true;
	}

	bool ParticleEmitter::Update(float deltaTime)
	{
		if (deltaTime <= 0)
		{
			return true;
		}
		bool result;
		KillParticles();
		EmitParticles(deltaTime);
		UpdateParticles(deltaTime);
		result = UpdateBuffers();
		return true;
	}

	bool ParticleEmitter::InitializeParticleSystem()
	{
		// Set the random deviation of where the particles can be located when emitted.
		mParticleDeviationX = 1.0f;
		mParticleDeviationY = 0.1f;
		m_particleDeviationZ = 1.0f;

		// Set the speed and speed variation of particles.
		mParticleVelocity = { 0.0f, 1.0f, 0.0f };
		mParticleVelocityVariation = { 0.0f, 0.2f, 0.0f };

		// Set the physical size of the particles.
		mParticleSize = 0.2f;

		// Set the number of particles to emit per second.
		mParticlesPerSecond = 2.0f;

		// Set the maximum number of particles allowed in the particle system.
		mMaxParticles = 100;

		// Create the particle list.
		mParticleList.resize(mMaxParticles);

		// Initialize the particle list.
		for (int i = 0; i < mMaxParticles; i++)
		{
			mParticleList[i].active = false;
		}

		// Initialize the current particle count to zero since none are emitted yet.
		mCurrentParticleCount = 0;

		// Clear the initial accumulated time for the particle per second emission rate.
		mAccumulatedTime = 0.0f;

		return true;
	}

	bool ParticleEmitter::InitializeBuffer()
	{
		MeshData* mesh = mParticlesMesh->GetOriginalMeshDataPtr();
		mesh->Indices.resize(mMaxParticles * 6u);
		for (int i = 0; i < mMaxParticles * 6; i++)
		{
			mesh->Indices[i] = i;
		}
		mesh->Vertices.resize(mMaxParticles * 6u);
		mParticlesMesh->InitializeBuffer();
		return true;
	}

	void ParticleEmitter::EmitParticles(float deltaTime)
	{
		bool emitParticle, found;
		float positionX, positionY, positionZ;
		Core::SVector3 velocity;
		float red, green, blue;
		int index;

		mAccumulatedTime += deltaTime * 1000;

		emitParticle = false;

		if (mAccumulatedTime > (1000.0f / mParticlesPerSecond))
		{
			mAccumulatedTime = 0.0f;
			emitParticle = true;
		}

		if (emitParticle && (mCurrentParticleCount < (mMaxParticles - 1)))
		{
			mCurrentParticleCount++;

			std::uniform_real_distribution<float> XDist(-mParticleDeviationX, mParticleDeviationX);
			std::uniform_real_distribution<float> YDist(0.0f, mParticleDeviationY);
			std::uniform_real_distribution<float> ZDist(-m_particleDeviationZ, m_particleDeviationZ);

			positionX = XDist(rng);
			positionY = YDist(rng);
			positionZ = ZDist(rng);

			velocity = mParticleVelocity + mParticleVelocityVariation * (((float)rand() - (float)rand()) / RAND_MAX);

			red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
			green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
			blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

			index = 0;
			found = false;
			while (!found)
			{
				if ((mParticleList[index].active == false) || (mParticleList[index].position.z < positionZ))
				{
					found = true;
				}
				else
				{
					index++;
				}
			}

			int i = mCurrentParticleCount;
			int j = i - 1;

			while (i != index)
			{
				mParticleList[i].position.x = mParticleList[j].position.x;
				mParticleList[i].position.y = mParticleList[j].position.y;
				mParticleList[i].position.z = mParticleList[j].position.z;
				mParticleList[i].color.x = mParticleList[j].color.x;
				mParticleList[i].color.y = mParticleList[j].color.y;
				mParticleList[i].color.z = mParticleList[j].color.z;
				mParticleList[i].color.w = mParticleList[j].color.w;
				mParticleList[i].velocity = mParticleList[j].velocity;
				mParticleList[i].active = mParticleList[j].active;
				i--;
				j--;
			}

			// Now insert it into the particle array in the correct depth order.
			mParticleList[index].position.x = positionX;
			mParticleList[index].position.y = positionY;
			mParticleList[index].position.z = positionZ;
			mParticleList[index].color.x = red;
			mParticleList[index].color.y = green;
			mParticleList[index].color.z = blue;
			mParticleList[index].color.w = 1.0f;
			mParticleList[index].velocity = velocity;
			mParticleList[index].active = true;
		}
	}

	void ParticleEmitter::UpdateParticles(float deltaTime)
	{
		int i;

		// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
		for (i = 0; i < mCurrentParticleCount; i++)
		{
			mParticleList[i].position.y = mParticleList[i].position.y - (mParticleList[i].velocity.y * deltaTime);
		}
	}

	void ParticleEmitter::KillParticles()
	{
		// Kill all the particles that have gone below a certain height range.
		for (int i = 0; i < mMaxParticles; i++)
		{
			if ((mParticleList[i].active == true) && (mParticleList[i].position.y < -5.0f))
			{
				mParticleList[i].active = false;
				mCurrentParticleCount--;

				// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
				for (int j = i; j < mMaxParticles - 1; j++)
				{
					mParticleList[j].position.x = mParticleList[j + 1].position.x;
					mParticleList[j].position.y = mParticleList[j + 1].position.y;
					mParticleList[j].position.z = mParticleList[j + 1].position.z;
					mParticleList[j].color.x = mParticleList[j + 1].color.x;
					mParticleList[j].color.y = mParticleList[j + 1].color.y;
					mParticleList[j].color.z = mParticleList[j + 1].color.z;
					mParticleList[j].color.w = mParticleList[j + 1].color.w;
					mParticleList[j].velocity = mParticleList[j + 1].velocity;
					mParticleList[j].active = mParticleList[j + 1].active;
				}
			}
		}
	}

	bool ParticleEmitter::UpdateBuffers()
	{
		int index = 0;
		MeshData* mesh = mParticlesMesh->GetOriginalMeshDataPtr();
		std::vector<Vertex>& vertices = mesh->Vertices;
		vertices.clear();
		vertices.resize(mMaxParticles * 6u, Vertex());
		for (int i = 0; i < mCurrentParticleCount; i++)
		{
			// Bottom left.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x - mParticleSize, mParticleList[i].position.y - mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(0.0f, 1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Top left.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x - mParticleSize, mParticleList[i].position.y + mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(0.0f, 0.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Bottom right.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x + mParticleSize, mParticleList[i].position.y - mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(1.0f, 1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Bottom right.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x + mParticleSize, mParticleList[i].position.y - mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(1.0f, 1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Top left.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x - mParticleSize, mParticleList[i].position.y + mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(0.0f, 0.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Top right.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x + mParticleSize, mParticleList[i].position.y + mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(1.0f, 0.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;
		}

		// ¸üÐÂ¶¥µã»º³å
		mParticlesMesh->UpdateBuffer();

		return true;
	}
}
