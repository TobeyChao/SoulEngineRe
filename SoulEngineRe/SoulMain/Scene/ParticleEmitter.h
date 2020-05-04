#pragma once
#include "../SoulPCH.h"
#include "../Core/Mathematics/SVector3.h"
#include "../Core/Mathematics/SVector4.h"
#include <random>

namespace Soul
{
	class ParticleEmitter
	{
	private:

		struct ParticleType
		{
			Core::SVector3 position;
			Core::SVector4 color;
			Core::SVector3 velocity;
			bool active;
		};

	public:
		ParticleEmitter();
		~ParticleEmitter();
		// 初始化需要指定粒子的材质名称
		bool Initialize(SubMesh* particlesMesh);
		bool Update(float deltaTime);
	private:

		bool InitializeParticleSystem();

		bool InitializeBuffer();

		void EmitParticles(float);
		void UpdateParticles(float);
		void KillParticles();

		bool UpdateBuffers();

	private:
		float mParticleDeviationX, mParticleDeviationY, m_particleDeviationZ;
		Core::SVector3 mParticleVelocity, mParticleVelocityVariation;
		float mParticleSize, mParticlesPerSecond;
		size_t mMaxParticles;

		int mCurrentParticleCount;
		float mAccumulatedTime;

		// 所有粒子
		std::vector<ParticleType> mParticleList;

		std::mt19937 rng;
		// 所有粒子的Mesh
		SubMesh* mParticlesMesh;
	};
}