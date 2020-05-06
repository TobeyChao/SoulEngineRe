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

	bool ParticleEmitter::Initialize(SubMesh* particlesMesh, const json& createParameters)
	{
		mParticlesMesh = particlesMesh;

		mParticleEmitCenter = { 0.f, 0.f , 0.f };
		if (createParameters.contains("particle_emit_center_x"))
		{
			mParticleEmitCenter.x = createParameters["particle_emit_center_x"];
		}
		if (createParameters.contains("particle_emit_center_y"))
		{
			mParticleEmitCenter.y = createParameters["particle_emit_center_y"];
		}
		if (createParameters.contains("particle_emit_center_z"))
		{
			mParticleEmitCenter.z = createParameters["particle_emit_center_z"];
		}

		// Set the random deviation of where the particles can be located when emitted.
		mParticleDeviation = { 1.0f, 1.0f, 1.0f };
		if (createParameters.contains("particle_emit_deviation_x"))
		{
			mParticleDeviation.x = createParameters["particle_emit_deviation_x"];
		}
		if (createParameters.contains("particle_emit_deviation_y"))
		{
			mParticleDeviation.y = createParameters["particle_emit_deviation_y"];
		}
		if (createParameters.contains("particle_emit_deviation_z"))
		{
			mParticleDeviation.z = createParameters["particle_emit_deviation_z"];
		}

		// Set the speed and speed variation of particles.
		mParticleVelocityVariationAngle = { 0.0f, 0.0f };
		mParticleVelocity = { 0.0f, 0.0f, 1.0f };
		if (createParameters.contains("particle_velocity_x"))
		{
			mParticleVelocity.x = createParameters["particle_velocity_x"];
		}
		if (createParameters.contains("particle_velocity_y"))
		{
			mParticleVelocity.y = createParameters["particle_velocity_y"];
		}
		if (createParameters.contains("particle_velocity_z"))
		{
			mParticleVelocity.z = createParameters["particle_velocity_z"];
		}
		if (createParameters.contains("particle_velocity_variation_angle_x"))
		{
			mParticleVelocityVariationAngle.x = createParameters["particle_velocity_variation_angle_x"];
		}
		if (createParameters.contains("particle_velocity_variation_angle_y"))
		{
			mParticleVelocityVariationAngle.y = createParameters["particle_velocity_variation_angle_y"];
		}

		mParticleLifeTimeMin = 2.0f;
		mParticleLifeTimeMax = 4.0f;
		if (createParameters.contains("particle_lifetime_min"))
		{
			mParticleLifeTimeMin = createParameters["particle_lifetime_min"];
		}
		if (createParameters.contains("particle_lifetime_max"))
		{
			mParticleLifeTimeMax = createParameters["particle_lifetime_max"];
		}

		// Set the physical size of the particles.
		mParticleSize = 0.2f;
		if (createParameters.contains("particle_size"))
		{
			mParticleSize = createParameters["particle_size"];
		}

		// Set the number of particles to emit per second.
		mParticlesPerSecond = 30.0f;
		if (createParameters.contains("particles_per_second"))
		{
			mParticlesPerSecond = createParameters["particles_per_second"];
		}

		// Set the number of seconds to emit per particle.
		mSecondsPerParticle = 1.0f / mParticlesPerSecond;

		// Set the maximum number of particles allowed in the particle system.
		mMaxParticles = 100;
		if (createParameters.contains("particles_max_num"))
		{
			mMaxParticles = createParameters["particles_max_num"];
		}

		// Create the particle list.
		mParticleList.resize(mMaxParticles);

		// Initialize the current particle count to zero since none are emitted yet.
		mCurrentParticleCount = 0;

		// Clear the initial accumulated time for the particle per second emission rate.
		mAccumulatedTime = 0.0f;

		return InitializeBuffer();
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
		float lifeTime;
		float red, green, blue;
		int index;

		mAccumulatedTime += deltaTime;

		emitParticle = false;

		if (mAccumulatedTime > (mSecondsPerParticle))
		{
			mAccumulatedTime = 0.0f;
			emitParticle = true;
		}

		if (emitParticle && (mCurrentParticleCount < (mMaxParticles - 1)))
		{
			mCurrentParticleCount++;

			std::uniform_real_distribution<float> XDist(
				mParticleEmitCenter.x - mParticleDeviation.x,
				mParticleEmitCenter.x + mParticleDeviation.x);
			std::uniform_real_distribution<float> YDist(
				mParticleEmitCenter.y - mParticleDeviation.y,
				mParticleEmitCenter.y + mParticleDeviation.y);
			std::uniform_real_distribution<float> ZDist(
				mParticleEmitCenter.z - mParticleDeviation.z,
				mParticleEmitCenter.z + mParticleDeviation.z);

			positionX = XDist(rng);
			positionY = YDist(rng);
			positionZ = ZDist(rng);

			std::uniform_real_distribution<float> angleXDist(
				-mParticleVelocityVariationAngle.x,
				mParticleVelocityVariationAngle.x);
			std::uniform_real_distribution<float> angleYDist(
				-mParticleVelocityVariationAngle.y,
				mParticleVelocityVariationAngle.y);
			
			float theta = angleYDist(rng);
			float phi = angleXDist(rng);

			velocity = mParticleVelocity * Core::MatrixRotationRollPitchYaw(theta, phi, 0.0f);

			lifeTime = mParticleLifeTimeMin +
				(mParticleLifeTimeMax - mParticleLifeTimeMin) *
				fabsf(((float)rand() - (float)rand()) / RAND_MAX);

			red = 1.f/*(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f*/;
			green = 1.f/*(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f*/;
			blue = 1.f/*(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f*/;

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
				mParticleList[i].lifeTime = mParticleList[j].lifeTime;
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
			mParticleList[index].color.w = 0.7f;
			mParticleList[index].velocity = velocity;
			mParticleList[index].lifeTime = lifeTime;
			mParticleList[index].active = true;
		}
	}

	void ParticleEmitter::UpdateParticles(float deltaTime)
	{
		int i;
		// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
		for (i = 0; i < mCurrentParticleCount; i++)
		{
			mParticleList[i].position = mParticleList[i].position + (mParticleList[i].velocity * deltaTime);
			mParticleList[i].lifeTime -= deltaTime;
		}
	}

	void ParticleEmitter::KillParticles()
	{
		// Kill all the particles that have gone below a certain height range.
		for (int i = 0; i < mMaxParticles; i++)
		{
			if ((mParticleList[i].active == true) && (mParticleList[i].lifeTime <= 0.0f))
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
					mParticleList[j].lifeTime = mParticleList[j + 1].lifeTime;
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
			vertices[index].Normal = Core::SVector3(0.0f, 0.0f, -1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Top left.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x - mParticleSize, mParticleList[i].position.y + mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(0.0f, 0.0f);
			vertices[index].Normal = Core::SVector3(0.0f, 0.0f, -1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Bottom right.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x + mParticleSize, mParticleList[i].position.y - mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(1.0f, 1.0f);
			vertices[index].Normal = Core::SVector3(0.0f, 0.0f, -1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Bottom right.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x + mParticleSize, mParticleList[i].position.y - mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(1.0f, 1.0f);
			vertices[index].Normal = Core::SVector3(0.0f, 0.0f, -1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Top left.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x - mParticleSize, mParticleList[i].position.y + mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(0.0f, 0.0f);
			vertices[index].Normal = Core::SVector3(0.0f, 0.0f, -1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;

			// Top right.
			vertices[index].Position = Core::SVector3(mParticleList[i].position.x + mParticleSize, mParticleList[i].position.y + mParticleSize, mParticleList[i].position.z);
			vertices[index].TexCoord = Core::SVector2(1.0f, 0.0f);
			vertices[index].Normal = Core::SVector3(0.0f, 0.0f, -1.0f);
			vertices[index].Color = mParticleList[i].color;
			index++;
		}

		// ¸üÐÂ¶¥µã»º³å
		mParticlesMesh->UpdateBuffer();

		return true;
	}
}
