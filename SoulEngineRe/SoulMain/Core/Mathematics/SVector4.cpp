#include "SVector4.h"
#include "SVector3.h"

namespace Soul
{
	namespace Core
	{
		SVector4::SVector4()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		SVector4::SVector4(float in_x, float in_y, float in_z, float in_w)
			:
			x(in_x),
			y(in_y),
			z(in_z),
			w(in_w)
		{}
		SVector4::SVector4(const SVector4& vec4)
		{
			x = vec4.x;
			y = vec4.y;
			z = vec4.z;
			w = vec4.w;
		}
		float SVector4::PlaneMagnitude() const
		{
			return sqrt(x * x + y * y + z * z);
		}
		void SVector4::PlaneNormalize()
		{
			float length = PlaneMagnitude();
			length = 1.0f / length;
			if (length > 0.0f)
			{
				(*this) *= length;
			}
		}
		float SVector4::PlaneEquationGetResultFrom(const SVector3& vec3)
		{
			return x * vec3.x + y * vec3.y + z * vec3.z + w;
		}
		SVector4& SVector4::operator=(const SVector4& vec4)
		{
			x = vec4.x;
			y = vec4.y;
			z = vec4.z;
			w = vec4.w;
			return *this;
		}
		SVector4 SVector4::operator+(const SVector4& v) const
		{
			return SVector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		SVector4 SVector4::operator-() const
		{
			return SVector4(-x, -y, -z, -w);
		}
		SVector4 SVector4::operator-(const SVector4& v) const
		{
			return SVector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		SVector4 SVector4::operator*(float num) const
		{
			return SVector4(x * num, y * num, z * num, w * num);
		}
		SVector4 SVector4::operator/(float num) const
		{
			return SVector4(x / num, y / num, z / num, w / num);
		}
		SVector4& SVector4::operator+=(const SVector4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}
		SVector4& SVector4::operator-=(const SVector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}
		SVector4& SVector4::operator*=(float num)
		{
			x *= num;
			y *= num;
			z *= num;
			w *= num;
			return *this;
		}
		SVector4& SVector4::operator/=(float num)
		{
			x /= num;
			y /= num;
			z /= num;
			w /= num;
			return *this;
		}
		bool SVector4::operator!=(const SVector4& v) const
		{
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}
		std::ostream& operator<<(std::ostream& output, const SVector4& vec4)
		{
			output << "X: " << vec4.x << " Y: " << vec4.y << " Z: " << vec4.z << " W: " << vec4.w;
			return output;
		}
		float Dot(const SVector4& vec4L, const SVector4& vec4R)
		{
			return vec4L.x * vec4R.x + vec4L.y * vec4R.y + vec4L.z * vec4R.z + vec4L.w * vec4R.w;
		}
	}
}