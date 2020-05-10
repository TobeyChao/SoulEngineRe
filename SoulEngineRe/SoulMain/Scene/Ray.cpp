#include "Ray.h"
#include "../RenderSystem/Viewport.h"
#include "../Core/Mathematics/SVector2.h"

namespace Soul
{
	Ray::Ray(const SceneNodeCamera& camera)
		:
		mCamera(camera),
		mOrigin{},
		mDirection{}
	{
	}
	Ray::Ray(const SceneNodeCamera& camera, const Core::SVector3& origin, const Core::SVector3& direction)
		:
		mCamera(camera),
		mOrigin(origin),
		mDirection(direction)
	{
	}
	Ray Ray::ScreenToRay(const SceneNodeCamera& camera, const Core::SVector2& screenPos)
	{
		return ScreenToRay(camera, screenPos.x, screenPos.y);
	}
	Ray Ray::ScreenToRay(const SceneNodeCamera& camera, float mouseX, float mouseY)
	{
		Core::SVector3 direction;

		Viewport* viewport = camera.GetViewport();
		const Core::SMatrix4x4& projectionMatrix = camera.GetProjectionMatrix();
		// 计算在相机空间的射线
		float vx = (2.0f * (float)mouseX / viewport->GetViewportWidth() - 1.0f) / projectionMatrix.mat[0][0];
		float vy = (-2.0f * (float)mouseY / viewport->GetViewportHeight() + 1.0f) / projectionMatrix.mat[1][1];
		
		direction = { vx, vy, 1.0f };
		Core::SMatrix4x4 invViewMatrix =  Core::MatrixInvViewMatrix(camera.GetViewMatrix());
		direction = direction * invViewMatrix;
		direction.Normalize();
		return Ray(camera, camera.GetPosition(), direction);
	}
	bool Ray::Hit(const BoundingBox& box, float* pOutDist, float maxDist)
	{
		// 摄像机空间的包围盒
		//BoundingBox boxInCamera;
		//box.Transform(boxInCamera, mCamera.GetViewMatrix());

		float tmin = 0.0f;
		float tmax = FLT_MAX;
		Core::SVector3 boxMin = box.mCenter - box.mLengthToSides;
		Core::SVector3 boxMax = box.mCenter + box.mLengthToSides;

		//The plane perpendicular to x-axie
		if (fabs(mDirection.x) < 0.000001f) //If the ray parallel to the plane
		{
			//If the ray is not within AABB box, then not intersecting
			if (mOrigin.x < boxMin.x || mOrigin.x > boxMax.x)
				return false;
		}
		else
		{
			//Compute the distance of ray to the near plane and far plane
			float ood = 1.0f / mDirection.x;
			float t1 = (boxMin.x - mOrigin.x) * ood;
			float t2 = (boxMax.x - mOrigin.x) * ood;

			//Make t1 be intersecting with the near plane, t2 with the far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			//Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			//Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}// end for perpendicular to x-axie

		//The plane perpendicular to y-axie
		if (abs(mDirection.y) < 0.000001f) //If the ray parallel to the plane
		{
			//If the ray is not within AABB box, then not intersecting
			if (mOrigin.y < boxMin.y || mOrigin.y > boxMax.y)
				return false;
		}
		else
		{
			//Compute the distance of ray to the near plane and far plane
			float ood = 1.0f / mDirection.y;
			float t1 = (boxMin.y - mOrigin.y) * ood;
			float t2 = (boxMax.y - mOrigin.y) * ood;

			//Make t1 be intersecting with the near plane, t2 with the far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			//Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			//Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}// end for perpendicular to y-axie

		//The plane perpendicular to z-axie
		if (abs(mDirection.z) < 0.000001f) //If the ray parallel to the plane
		{
			//If the ray is not within AABB box, then not intersecting
			if (mOrigin.z < boxMin.z || mOrigin.z > boxMax.z)
				return false;
		}
		else
		{
			//Compute the distance of ray to the near plane and far plane
			float ood = 1.0f / mDirection.z;
			float t1 = (boxMin.z - mOrigin.z) * ood;
			float t2 = (boxMax.z - mOrigin.z) * ood;

			//Make t1 be intersecting with the near plane, t2 with the far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			//Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			//Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}// end for perpendicular to z-axie

		Core::SVector3 hitPosInCamera = mOrigin + mDirection * tmin;
		// 距离
		float dist = hitPosInCamera.Magnitude();
		if (pOutDist)
		{
			*pOutDist = dist;
		}
		// 距离判定
		if (dist > maxDist)
		{
			return false;
		}
		return true;
	}
}
