#pragma once
#include "MathPCH.h"
namespace Soul
{
	namespace Core
	{
		class SQuaternion
		{
		public:
			SQuaternion();
			SQuaternion(float _w, float _x, float _y, float _z);
			SQuaternion(const SQuaternion& quaternion);

		public:
			float w;
			float x;
			float y;
			float z;
		};
		SQuaternion QuaternionIdentity();
		SQuaternion QuaternionRotationX(float angle);
		SQuaternion QuaternionRotationY(float angle);
		SQuaternion QuaternionRotationZ(float angle);
		SQuaternion QuaternionRotationAboutAxis(const SVector3& axis, float angle);
		SQuaternion QuaternionRotationObjectToInertial(float pitch, float yaw, float roll);
	}
}