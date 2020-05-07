#include "SVector3.h"

namespace Soul
{
	namespace Core
	{
		SVector3::SVector3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		SVector3::SVector3(float in_x, float in_y, float in_z)
			:
			x(in_x),
			y(in_y),
			z(in_z)
		{}
		SVector3::SVector3(const SVector3& vec3)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
		}
		SVector3& SVector3::operator=(const SVector3& vec3)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			return *this;
		}
		float SVector3::Magnitude() const
		{
			return sqrt(x * x + y * y + z * z);
		}
		void SVector3::Normalize()
		{
			float length = Magnitude();
			if (length != 0.0f)
			{
				(*this) /= length;
			}
		}
		float SVector3::Dot(const SVector3& vec3) const
		{
			return x * vec3.x + y * vec3.y + z * vec3.z;
		}
		SVector3 SVector3::Cross(const SVector3& vec3) const
		{
			return SVector3(
			(y * vec3.z) - (z * vec3.y),
				(z * vec3.x) - (x * vec3.z),
				(x * vec3.y) - (y * vec3.x));
		}
		SVector3 SVector3::MidPoint(const SVector3& vec3) const
		{
			return SVector3(
			(x + vec3.x) * 0.5f,
				(y + vec3.y) * 0.5f,
				(z + vec3.z) * 0.5f);
		}
		SVector3 SVector3::operator+(const SVector3& vec3) const
		{
			return SVector3(x + vec3.x, y + vec3.y, z + vec3.z);
		}
		SVector3 SVector3::operator-() const
		{
			return SVector3(-x, -y, -z);
		}
		SVector3 SVector3::operator-(const SVector3& vec3) const
		{
			return SVector3(x - vec3.x, y - vec3.y, z - vec3.z);
		}
		SVector3 SVector3::operator*(float num) const
		{
			return SVector3(x * num, y * num, z * num);
		}
		SVector3 SVector3::operator/(float num) const
		{
			return SVector3(x / num, y / num, z / num);
		}
		SVector3& SVector3::operator+=(const SVector3& vec3)
		{
			x += vec3.x;
			y += vec3.y;
			z += vec3.z;
			return *this;
		}
		SVector3& SVector3::operator-=(const SVector3& vec3)
		{
			x -= vec3.x;
			y -= vec3.y;
			z -= vec3.z;
			return *this;
		}
		SVector3& SVector3::operator*=(float num)
		{
			x *= num;
			y *= num;
			z *= num;
			return *this;
		}
		SVector3& SVector3::operator/=(float num)
		{
			x /= num;
			y /= num;
			z /= num;
			return *this;
		}
		bool SVector3::operator!=(const SVector3& vec3) const
		{
			return x != vec3.x || y != vec3.y || z != vec3.z;
		}
		/*************non-member-function*************/
		std::ostream& operator<<(std::ostream& output, const SVector3& vec3)
		{
			output << "X: " << vec3.x << " Y: " << vec3.y << " Z: " << vec3.z;
			return output;
		}
		float Dot(const SVector3& vec3L, const SVector3& vec3R)
		{
			return vec3L.x * vec3R.x + vec3L.y * vec3R.y + vec3L.z * vec3R.z;
		}
		SVector3 Cross(const SVector3& vec3L, const SVector3& vec3R)
		{
			return SVector3(
			(vec3L.y * vec3R.z) - (vec3L.z * vec3R.y),
				(vec3L.z * vec3R.x) - (vec3L.x * vec3R.z),
				(vec3L.x * vec3R.y) - (vec3L.y * vec3R.x));
		}
		SVector3 Multiply(const SVector3& vec3L, const SVector3& vec3R)
		{
			return { vec3L.x * vec3R.x, vec3L.y * vec3R.y, vec3L.z * vec3R.z };
		}
		float Magnitude(const SVector3& vec3)
		{
			return sqrt(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z);
		}
		void Normalize(SVector3& vec3)
		{
			vec3.Normalize();
		}
	}
}