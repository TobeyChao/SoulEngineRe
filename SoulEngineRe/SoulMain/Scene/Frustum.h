#pragma once
#include "BoundingBox.h"
#include "../Core/Mathematics/SMatrix4x4.h"
#include "../Core/Mathematics/SVector4.h"

namespace Soul
{
	class Frustum
	{
	public:
		Frustum();
		//判断一个点是否在视截体内
		bool CheckPoint(float x, float y, float z);
		//判断一个立方体是否在视截体内，用的是包围球的办法
		bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
		//判断一个球体是否在视截体内,用的是包围球的办法
		bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
		//判断一个长方体是否在视截体内，用的也是包围球的的方法
		bool CheckRectangle(const BoundingBox& boundingBox);
		bool CheckRectangle(const Core::SVector3& center, const Core::SVector3& lengthToSides);
		bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
		//判断一个长方体是否在视截体内，用的是8点的方法
		bool CheckRectangle2(float, float, float, float, float, float);

	protected:
		//根据屏幕的深度，投影矩阵和相机矩阵求出相应的视截体的6个平面
		void BuildFrustum(const Core::SMatrix4x4& projMatrix, const Core::SMatrix4x4& viewMatrix);

	private:
		bool mFirstBuild;
		Core::SMatrix4x4 mCacheViewMatrix;
		Core::SMatrix4x4 mCacheProjMatrix;
		Core::SVector4 mPlane[6];
	};
}