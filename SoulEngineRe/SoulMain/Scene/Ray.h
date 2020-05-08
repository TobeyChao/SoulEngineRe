#pragma once
#include "../Scene/BoundingBox.h"
#include "../Scene/SceneNodeCamera.h"

namespace Soul
{
	class Ray
	{
	public:
		Ray(const SceneNodeCamera& camera);
		Ray(const SceneNodeCamera& camera, const Core::SVector3& origin, const Core::SVector3& direction);
		~Ray() = default;
		static Ray ScreenToRay(const SceneNodeCamera& camera, const Core::SVector2& screenPos);
		static Ray ScreenToRay(const SceneNodeCamera& camera, float screenX, float screenY);
		bool Hit(const BoundingBox& box, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	public:
		Core::SVector3 mOrigin;			// 射线原点
		Core::SVector3 mDirection;		// 单位方向向量
		const SceneNodeCamera& mCamera;	// 发射射线的摄像机
	};
}