#pragma once
#include <iostream>
#include "MathPCH.h"

namespace Soul
{
	namespace Core
	{
		class SMatrix4x4;
		class SVector3
		{
		public:
			SVector3();
			SVector3(float in_x, float in_y, float in_z);
			SVector3(const SVector3& vec3);
			SVector3& operator=(const SVector3& vec3);

			float Magnitude() const;
			void Normalize();
			float Dot(const SVector3& vec3) const;
			SVector3 Cross(const SVector3& vec3) const;
			SVector3 MidPoint(const SVector3& vec3) const;

			//Operators
			friend std::ostream& operator<<(std::ostream& output, const SVector3& vec3);
			SVector3 operator+(const SVector3& v) const;
			SVector3 operator-() const;
			SVector3 operator-(const SVector3& v) const;
			SVector3 operator*(float num) const;
			SVector3 operator/(float num) const;
			SVector3& operator+=(const SVector3& v);
			SVector3& operator-=(const SVector3& v);
			SVector3& operator*=(float num);
			SVector3& operator/=(float num);
			//Returns true if vectors different.
			bool operator!=(const SVector3& v) const;
		public:
			float x;
			float y;
			float z;
		};

		float Dot(const SVector3& vec3L, const SVector3& vec3R);
		SVector3 Cross(const SVector3& vec3L, const SVector3& vec3R);
		SVector3 Multiply(const SVector3& vec3L, const SVector3& vec3R);
		float Magnitude(const SVector3& vec3);
		void Normalize(SVector3& vec3);
	}
}