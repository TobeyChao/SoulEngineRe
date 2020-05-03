#include "SVector2.h"

namespace Soul
{
	namespace Core
	{
		SVector2::SVector2()
			:
			x(0.0f),
			y(0.0f)
		{}
		SVector2::SVector2(float x_in, float y_in)
			:
			x(x_in),
			y(y_in)
		{}
		SVector2::SVector2(const SVector2& vec2)
			:
			x(vec2.x),
			y(vec2.y)
		{}
		float SVector2::Magnitude() const
		{
			return (float)sqrt(x * x + y * y);
		}
		void SVector2::Normalize()
		{
			float length = Magnitude();
			if (length != 0.0f)
			{
				(*this) /= length;
			}
		}
		SVector2 SVector2::MidPoint(const SVector2& vec2) const
		{
			return SVector2((x + vec2.x) * 0.5f, (y + vec2.y) * 0.5f);
		}
		SVector2& SVector2::operator=(const SVector2& vec2)
		{
			x = vec2.x;
			y = vec2.y;
			return *this;
		}
		SVector2 SVector2::operator-() const
		{
			return SVector2(-x, -y);
		}
		SVector2& SVector2::operator-= (const SVector2& vec2)
		{
			x -= vec2.x;
			y -= vec2.y;
			return *this;
		}
		SVector2& SVector2::operator+= (const SVector2& vec2)
		{
			x += vec2.x;
			y += vec2.y;
			return *this;
		}
		SVector2& SVector2::operator*=(const float n)
		{
			x *= n;
			y *= n;
			return *this;
		}
		SVector2& SVector2::operator/=(const float n)
		{
			x /= n;
			y /= n;
			return *this;
		}
		SVector2 SVector2::operator+(const SVector2& vec2) const
		{
			return SVector2(*this) += vec2;
		}
		SVector2 SVector2::operator-(const SVector2& vec2) const
		{
			return SVector2(*this) -= vec2;
		}
		SVector2 SVector2::operator*(const float n)
		{
			return SVector2(*this) *= n;
		}
		SVector2 SVector2::operator/(const float n)
		{
			return SVector2(*this) /= n;
		}
		bool SVector2::operator!=(const SVector2& vec2) const
		{
			return x != vec2.x || y != vec2.y;
		}
		/*************non-member-function*************/
		float Cross(const SVector2& vec2L, const SVector2& vec2R)
		{
			return vec2L.x * vec2R.y - vec2L.y * vec2R.x;
		}
		float Dot(const SVector2& vec2L, const SVector2& vec2R)
		{
			return vec2L.x * vec2R.x + vec2L.y * vec2R.y;
		}
	}
}