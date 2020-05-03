#include "SQuaternion.h"
#include "SMatrix4x4.h"
#include "SVector3.h"

namespace Soul
{
	namespace Core
	{
		SQuaternion::SQuaternion()
			:
			w(1.0f),
			x(0.0f),
			y(0.0f),
			z(0.0f)
		{
		}
		SQuaternion::SQuaternion(float _w, float _x, float _y, float _z)
			:
			w(_w),
			x(_x),
			y(_y),
			z(_z)
		{
		}
		SQuaternion::SQuaternion(const SQuaternion& quaternion)
		{
			w = quaternion.w;
			x = quaternion.x;
			y = quaternion.y;
			z = quaternion.z;
		}
		SQuaternion QuaternionIdentity()
		{
			return SQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
		}
		SQuaternion QuaternionRotationX(float angle)
		{
			float halfAngle = angle * 0.5f;
			return SQuaternion(cosf(halfAngle), sinf(halfAngle), 0.0f, 0.0f);
		}
		SQuaternion QuaternionRotationY(float angle)
		{
			float halfAngle = angle * 0.5f;
			return SQuaternion(cosf(halfAngle), 0.0f, sinf(halfAngle), 0.0f);
		}
		SQuaternion QuaternionRotationZ(float angle)
		{
			float halfAngle = angle * 0.5f;
			return SQuaternion(cosf(halfAngle), 0.0f, 0.0f, sinf(halfAngle));
		}
		SQuaternion QuaternionRotationAboutAxis(const SVector3& axis, float angle)
		{
			float halfAngle = angle * 0.5f;
			float sinHalfAngle = sinf(halfAngle);
			return SQuaternion(cosf(halfAngle),
				axis.x * sinHalfAngle, axis.y * sinHalfAngle, axis.z * sinHalfAngle);
		}
		SQuaternion QuaternionRotationObjectToInertial(float pitch, float yaw, float roll)
		{
			//From EulerAngle To Quaternion
			SVector3 angle(pitch, yaw, roll);
			SVector3 halfAngle = angle * 0.5f;
			float sinP, sinY, sinR;
			float cosP, cosY, cosR;
			ScalarSinCos(&sinP, &cosP, halfAngle.x);
			ScalarSinCos(&sinY, &cosY, halfAngle.y);
			ScalarSinCos(&sinR, &cosR, halfAngle.z);
			float w = cosY * cosP * cosR + sinY * sinP * sinR;
			float x = cosY * sinP * cosR + sinY * cosP * sinR;
			float y = -cosY * sinP * sinR + sinY * cosP * cosR;
			float z = -sinY * sinP * cosR + cosY * cosP * sinR;
			return SQuaternion(w, x, y, z);
		}
	}
}