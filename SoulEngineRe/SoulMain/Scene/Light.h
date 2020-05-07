#pragma once
#include "GameObject.h"
#include "../Core/Mathematics/SVector3.h"
#include "../Core/Mathematics/SVector4.h"

namespace Soul
{
	enum class LIGHT_TYPE
	{
		LT_NONE,
		LT_DIRECTIONAL,
		LT_POINT,
		LT_SPOT
	};

	struct DirectionalLight
	{
		Core::SVector4 Ambient;		// 环境光
		Core::SVector4 Diffuse;		// 漫反射
		Core::SVector4 Specular;	// 镜面光
		Core::SVector3 Direction;	// 方向
		float Pad = 0.0f;
	};

	struct PointLight
	{
		Core::SVector4 Ambient;		// 环境光
		Core::SVector4 Diffuse;		// 漫反射
		Core::SVector4 Specular;	// 镜面光

		Core::SVector3 Position;	// 位置
		float Range = 0.0f;			// 范围

		Core::SVector3 Att;			// 衰减
		float Pad = 0.0f;
	};

	struct SpotLight
	{
		Core::SVector4 Ambient;		// 环境光
		Core::SVector4 Diffuse;		// 漫反射
		Core::SVector4 Specular;	// 镜面光

		Core::SVector3 Position;
		float Range = 0.0f;			// 范围

		Core::SVector3 Direction;
		float Spot = 0.0f;			// 聚光强度

		Core::SVector3 Att;
		float Pad = 0.0f;
	};

	// 光源基类
	class Light : public GameObject
	{
	public:
		Light(const std::string& name, LIGHT_TYPE lightType)
			:
			GameObject(name),
			mLightType(lightType),
			mName(name)
		{
		}
		LIGHT_TYPE GetType() const
		{
			return mLightType;
		}
		void SetAmbient(const Core::SVector4& ambient)
		{
			mAmbient = ambient;
		}
		void SetDiffuse(const Core::SVector4& diffuse)
		{
			mDiffuse = diffuse;
		}
		void SetSpecular(const Core::SVector4& specular)
		{
			mSpecular = specular;
		}
		void SetDirection(const Core::SVector3& direction)
		{
			mDirection = direction;
		}
		void SetPosition(const Core::SVector3& position)
		{
			mPosition = position;
		}
		void SetRange(float range)
		{
			mRange = range;
		}
		void SetSpot(float spot)
		{
			mSpot = spot;
		}
		void SetAtt(const Core::SVector3& att)
		{
			mAtt = att;
		}

		const Core::SVector4& GetAmbient() const
		{
			return mAmbient;
		}
		const Core::SVector4& GetDiffuse() const
		{
			return mDiffuse;
		}
		const Core::SVector4& GetSpecular() const
		{
			return mSpecular;
		}
		const Core::SVector3& GetDirection() const
		{
			return mDirection;
		}
		const Core::SVector3& GetPosition() const
		{
			return mPosition;
		}
		float GetRange() const
		{
			return mRange;
		}
		float GetSpot() const
		{
			return mSpot;
		}
		const Core::SVector3& GetAtt() const
		{
			return mAtt;
		}
		static const int MaxLights = 3;
	private:
		std::string mName;

		LIGHT_TYPE mLightType;
		Core::SVector4 mAmbient;	// 环境光
		Core::SVector4 mDiffuse;	// 漫反射
		Core::SVector4 mSpecular;	// 镜面光
		Core::SVector3 mDirection;	// 方向
		Core::SVector3 mPosition;	// 位置
		float mRange = 0.0f;		// 范围
		Core::SVector3 mAtt;		// 衰减
		float mSpot = 0.0f;			// 聚光强度
	};
}