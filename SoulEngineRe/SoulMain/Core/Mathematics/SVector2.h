#pragma once
#include "MathPCH.h"

namespace Soul
{
	namespace Core
	{
		class SVector2
		{
		public:
			SVector2();
			SVector2(float x_in, float y_in);
			SVector2(const SVector2& vec2);

			float Magnitude() const;

			void Normalize();

			SVector2 MidPoint(const SVector2& vec2) const;

			SVector2& operator=(const SVector2& vec2);

			SVector2 operator-() const;

			SVector2& operator+= (const SVector2& vec2);

			SVector2& operator-= (const SVector2& vec2);

			SVector2& operator*= (const float n);

			SVector2& operator/= (const float n);

			SVector2  operator+ (const SVector2& vec2) const;

			SVector2  operator- (const SVector2& vec2) const;

			SVector2  operator* (const float n);

			SVector2  operator/ (const float n);

			bool operator!=(const SVector2& vec2) const;
		public:
			float x;
			float y;
		};
		/*************non-member-function*************/
		float Cross(const SVector2& vec2L, const SVector2& vec2R);
		float Dot(const SVector2& vec2L, const SVector2& vec2R);
	}
}