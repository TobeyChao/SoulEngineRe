#include "Frustum.h"

namespace Soul
{
	Frustum::Frustum()
		:
		mFirstBuild(true),
		mScreenDepth(0)
	{
	}

	//根据屏幕的深度，投影矩阵和相机矩阵求出世界空间的相应的视截体的6个平面
	void Frustum::BuildFrustum(float sceneDepth, const Core::SMatrix4x4& projMatrix, const Core::SMatrix4x4& viewMatrix)
	{
		mViewMatrix = viewMatrix;
		mProjMatrix = projMatrix;
		mScreenDepth = sceneDepth;

		// 如果不是第一次构建并且和上次的矩阵相同就不构建
		if (!mFirstBuild && (mViewMatrix == viewMatrix || mProjMatrix == projMatrix))
		{
			return;
		}

		float zMinimum, r;
		Core::SMatrix4x4 matrix;
		Core::SMatrix4x4 tempMatrix = mProjMatrix;

		//计算视截体近裁剪面的距离
		zMinimum = -tempMatrix.mat[3][2] / tempMatrix.mat[2][2];
		r = mScreenDepth / (mScreenDepth - zMinimum);

		tempMatrix.mat[2][2] = r;
		tempMatrix.mat[3][2] = -r * zMinimum;

		//从相机矩阵和投影矩阵计算视截体矩阵
		matrix = mViewMatrix * tempMatrix;;

		//计算视截体的近裁剪面
		Core::SVector4 nearPlane;
		nearPlane.x = matrix.mat[0][3] + matrix.mat[0][2];//_14 _13
		nearPlane.y = matrix.mat[1][3] + matrix.mat[1][2];//_24 _23
		nearPlane.z = matrix.mat[2][3] + matrix.mat[2][2];//_34 _33
		nearPlane.w = matrix.mat[3][3] + matrix.mat[3][2];//_44 _43
		mPlane[0] = nearPlane;
		mPlane[0].PlaneNormalize();

		//计算视截体的远裁剪面
		Core::SVector4 FarPlane;
		FarPlane.x = matrix.mat[0][3] - matrix.mat[0][2];//_14_13
		FarPlane.y = matrix.mat[1][3] - matrix.mat[1][2];//_24_23
		FarPlane.z = matrix.mat[2][3] - matrix.mat[2][2];//_34_33
		FarPlane.w = matrix.mat[3][3] - matrix.mat[3][2];//_44_43
		mPlane[1] = FarPlane;
		mPlane[1].PlaneNormalize();

		//计算视截体的左裁剪面(XZ面)
		Core::SVector4 LeftPlane;
		LeftPlane.x = matrix.mat[0][3] + matrix.mat[0][0];//_14_11
		LeftPlane.y = matrix.mat[1][3] + matrix.mat[1][0];//_24_21
		LeftPlane.z = matrix.mat[2][3] + matrix.mat[2][0];//_34_31
		LeftPlane.w = matrix.mat[3][3] + matrix.mat[3][0];//_44_41
		mPlane[2] = LeftPlane;
		mPlane[2].PlaneNormalize();

		//计算视截体的右裁剪面(XZ面)
		Core::SVector4 RightPlane;
		RightPlane.x = matrix.mat[0][3] - matrix.mat[0][0];//_14_11
		RightPlane.y = matrix.mat[1][3] - matrix.mat[1][0];//_24_21
		RightPlane.z = matrix.mat[2][3] - matrix.mat[2][0];//_34_31
		RightPlane.w = matrix.mat[3][3] - matrix.mat[3][0];//_44_41
		mPlane[3] = RightPlane;
		mPlane[3].PlaneNormalize();

		//计算视截体的顶裁剪面(YZ面)
		Core::SVector4 TopPlane;
		TopPlane.x = matrix.mat[0][3] - matrix.mat[0][1];//_14_12
		TopPlane.y = matrix.mat[1][3] - matrix.mat[1][1];//_24_22
		TopPlane.z = matrix.mat[2][3] - matrix.mat[2][1];//_34_32
		TopPlane.w = matrix.mat[3][3] - matrix.mat[3][1];//_44_42
		mPlane[4] = TopPlane;
		mPlane[4].PlaneNormalize();

		//计算视截体的底裁剪面(YZ面)
		Core::SVector4 BottomPlane;
		BottomPlane.x = matrix.mat[0][3] + matrix.mat[0][1];//_14_12
		BottomPlane.y = matrix.mat[1][3] + matrix.mat[1][1];//_24_22
		BottomPlane.z = matrix.mat[2][3] + matrix.mat[2][1];//_34_32
		BottomPlane.w = matrix.mat[3][3] + matrix.mat[3][1];//_44_42
		mPlane[5] = BottomPlane;
		mPlane[5].PlaneNormalize();
	}

	//判断一个点是否在视截体内
	//一个点若是视截体6个面的正面，则该点在视截体内
	bool Frustum::CheckPoint(float x, float y, float z)
	{
		Core::SVector3 Point = Core::SVector3(x, y, z);
		for (int i = 0; i < 6; ++i)
		{
			//点乘结果
			float DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point);
			if (DotEnd < 0.0f)
			{
				return false;
			}
		}
		return true;
	}

	//判断一个立方体是否在视截体内，radius=正方形边长/2
	//第一条，算法，只要构成立方体的8个点都位于视截体6个面中的任意一个面的负面，那么正方体就位于视截体外
	//不满足第一条则在视截体内(相交或者完全位于视截体)
	bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
	{
		Core::SVector3 Point[8];
		float DotEnd;
		Point[0] = Core::SVector3(xCenter - radius, yCenter - radius, zCenter - radius);
		Point[1] = Core::SVector3(xCenter + radius, yCenter - radius, zCenter - radius);
		Point[2] = Core::SVector3(xCenter - radius, yCenter + radius, zCenter - radius);
		Point[3] = Core::SVector3(xCenter - radius, yCenter - radius, zCenter + radius);
		Point[4] = Core::SVector3(xCenter + radius, yCenter + radius, zCenter - radius);
		Point[5] = Core::SVector3(xCenter + radius, yCenter - radius, zCenter + radius);
		Point[6] = Core::SVector3(xCenter - radius, yCenter + radius, zCenter + radius);
		Point[7] = Core::SVector3(xCenter + radius, yCenter + radius, zCenter + radius);

		for (int i = 0; i < 6; ++i)
		{
			/*分辨计算正方体的每个点是否在视截体内*/

			/*一*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[0]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*二*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[1]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*三*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[2]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*四*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[3]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*五*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[4]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*六*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[5]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*七*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[6]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*八*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[7]);
			if (DotEnd >= 0)
			{
				continue;
			}
			return false;
		}
		return true;
	}



	//判断一个球体是否在视截体内,用的是包围球的办法
	//由于构建视截体求出的6个面是单位方向向量，因此球心与平面的点积为球心到平面的距离
	//假设球心c到视截体6个平面中的任意一平面的距离为k,如果-r>k,则球体位于对应平面的反向之外，即球体完全位于视截体之外，其它情况球体与视截体相交(部分相交或者完全位于视截体)
	bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
	{
		Core::SVector3 Point = Core::SVector3(xCenter, yCenter, zCenter);
		float DotEnd;
		for (int i = 0; i < 6; ++i)
		{
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point);
			if (-radius > DotEnd)
			{
				return false;
			}
		}
		return true;
	}

	//算法和正方体的算法一样
	//xSize,ySize,zSize分别为长方体长宽高的一半
	bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
	{
		Core::SVector3 Point[8];
		float DotEnd;
		Point[0] = Core::SVector3(xCenter - xSize, yCenter - ySize, zCenter - zSize);
		Point[1] = Core::SVector3(xCenter + xSize, yCenter - ySize, zCenter - zSize);
		Point[2] = Core::SVector3(xCenter - xSize, yCenter + ySize, zCenter - zSize);
		Point[3] = Core::SVector3(xCenter - xSize, yCenter - ySize, zCenter + zSize);
		Point[4] = Core::SVector3(xCenter + xSize, yCenter + ySize, zCenter - zSize);
		Point[5] = Core::SVector3(xCenter + xSize, yCenter - ySize, zCenter + zSize);
		Point[6] = Core::SVector3(xCenter - xSize, yCenter + ySize, zCenter + zSize);
		Point[7] = Core::SVector3(xCenter + xSize, yCenter + ySize, zCenter + zSize);

		for (int i = 0; i < 6; ++i)
		{
			/*分辨计算正方体的每个点是否在视截体内*/

			/*一*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[0]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*二*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[1]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*三*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[2]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*四*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[3]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*五*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[4]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*六*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[5]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*七*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[6]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*八*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[7]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			return false;
		}
		return true;
	}

	bool Frustum::CheckRectangle2(float maxWidth, float maxHeight, float maxDepth, float minWidth, float minHeight, float minDepth)
	{
		Core::SVector3 Point[8];
		float DotEnd;
		Point[0] = Core::SVector3(minWidth, minHeight, minDepth);
		Point[1] = Core::SVector3(maxWidth, minHeight, minDepth);
		Point[2] = Core::SVector3(minWidth, maxHeight, minDepth);
		Point[3] = Core::SVector3(maxWidth, maxHeight, minDepth);
		Point[4] = Core::SVector3(minWidth, minHeight, maxDepth);
		Point[5] = Core::SVector3(maxWidth, minHeight, maxDepth);
		Point[6] = Core::SVector3(minWidth, maxHeight, maxDepth);
		Point[7] = Core::SVector3(maxWidth, maxHeight, maxDepth);

		for (int i = 0; i < 6; ++i)
		{
			/*分辨计算正方体的每个点是否在视截体内*/

			/*一*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[0]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*二*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[1]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*三*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[2]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*四*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[3]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*五*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[4]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*六*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[5]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*七*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[6]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*八*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[7]);
			if (DotEnd >= 0)
			{
				continue;
			}

			return false;
		}
		return true;
	}
}