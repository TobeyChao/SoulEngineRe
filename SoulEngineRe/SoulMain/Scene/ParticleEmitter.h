#pragma once
#include "../SoulPCH.h"
#include "../Core/Mathematics/SVector2.h"
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
			ParticleType()
				:
				lifeTime(0.0f),
				active(false)
			{

			}
			Core::SVector3 position;
			Core::SVector4 color;
			Core::SVector3 velocity;
			float lifeTime;
			bool active;
		};

	public:
		ParticleEmitter();
		~ParticleEmitter();
		bool Initialize(SubMesh* particlesMesh, const json& createParameters);
		bool Update(float deltaTime);
	private:

		bool InitializeBuffer();

		void EmitParticles(float);
		void UpdateParticles(float);
		void KillParticles();

		bool UpdateBuffers();

	private:
		// 粒子的发射中心
		Core::SVector3 mParticleEmitCenter;
		// 粒子的发射中心偏离
		Core::SVector3 mParticleDeviation;
		// 粒子速度，默认z轴正方向
		Core::SVector3 mParticleVelocity;
		// 粒子速度方向绕xy轴的最大角度
		Core::SVector2 mParticleVelocityVariationAngle;
		// 粒子寿命的范围
		float mParticleLifeTimeMin;
		float mParticleLifeTimeMax;
		// 粒子的大小
		float mParticleSize;
		// 每秒发射粒子数
		float mParticlesPerSecond;
		// 每发射一个粒子需要的秒数
		float mSecondsPerParticle;
		// 最大粒子数
		size_t mMaxParticles;
		// 当前存活粒子
		int mCurrentParticleCount;
		// 累积时间
		float mAccumulatedTime;
		// 所有粒子
		std::vector<ParticleType> mParticleList;
		// 随机数引擎
		std::mt19937 rng;
		// 粒子的Mesh
		SubMesh* mParticlesMesh;
	};
}