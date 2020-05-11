#pragma once
#include <iostream>
#include "MathPCH.h"

namespace Soul
{
	namespace Core
	{
		class SVector4
		{
		public:
			SVector4();
			SVector4(float in_x, float in_y, float in_z, float in_w);
			SVector4(const SVector4& vec4);

			float PlaneMagnitude() const;
			void PlaneNormalize();

			float PlaneEquationGetResultFrom(const SVector3& vec3);

			//Operators
			friend std::ostream& operator<<(std::ostream& output, const SVector4& vec4);
			SVector4& operator=(const SVector4& vec4);
			SVector4 operator+(const SVector4& v) const;
			SVector4 operator-() const;
			SVector4 operator-(const SVector4& v) const;
			SVector4 operator*(float num) const;
			SVector4 operator/(float num) const;
			SVector4& operator+=(const SVector4& v);
			SVector4& operator-=(const SVector4& v);
			SVector4& operator*=(float num);
			SVector4& operator/=(float num);
			//Returns true if vectors different.
			bool operator!=(const SVector4& v) const;
		public:
			float x;
			float y;
			float z;
			float w;
		};
		float Dot(const SVector4& vec4L, const SVector4& vec4R);
	}
}