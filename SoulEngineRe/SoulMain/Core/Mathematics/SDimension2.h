#pragma once
namespace Soul
{
	namespace Core
	{
		class SDimension2
		{
		public:
			unsigned int width;
			unsigned int height;

			SDimension2() = default;
			SDimension2(unsigned int x_in, unsigned int y_in);
			SDimension2(const SDimension2& dimension2);

			SDimension2& operator=(const SDimension2& dimension2);
			SDimension2& operator+=(const SDimension2& dimension2);
			SDimension2& operator-=(const SDimension2& dimension2);
			SDimension2& operator*=(unsigned int n);
			SDimension2& operator/=(unsigned int n);
			SDimension2 operator+(const SDimension2& dimension2) const;
			SDimension2 operator-(const SDimension2& dimension2) const;
			SDimension2 operator*(unsigned int n);
			SDimension2 operator/(unsigned int n);
		};
	}
}