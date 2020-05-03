#include "SDimension2.h"

namespace Soul
{
	namespace Core
	{
		SDimension2::SDimension2(unsigned int x_in, unsigned int y_in)
			:
			width(x_in),
			height(y_in)
		{}

		SDimension2::SDimension2(const SDimension2& dimension2)
			:
			width(dimension2.width),
			height(dimension2.height)
		{}

		SDimension2& SDimension2::operator=(const SDimension2& dimension2)
		{
			width = dimension2.width;
			height = dimension2.height;
			return *this;
		}
		SDimension2& SDimension2::operator+=(const SDimension2& dimension2)
		{
			width += dimension2.width;
			height += dimension2.height;
			return *this;
		}
		SDimension2& SDimension2::operator-=(const SDimension2& dimension2)
		{
			width -= dimension2.width;
			height -= dimension2.height;
			return *this;
		}
		SDimension2& SDimension2::operator*=(unsigned int n)
		{
			width *= n;
			height *= n;
			return *this;
		}
		SDimension2& SDimension2::operator/=(unsigned int n)
		{
			width /= n;
			height /= n;
			return *this;
		}
		SDimension2 SDimension2::operator+(const SDimension2& dimension2) const
		{
			return SDimension2(*this) += dimension2;
		}
		SDimension2 SDimension2::operator-(const SDimension2& dimension2) const
		{
			return SDimension2(*this) -= dimension2;
		}
		SDimension2 SDimension2::operator*(unsigned int n)
		{
			return SDimension2(*this) *= n;
		}
		SDimension2 SDimension2::operator/(unsigned int n)
		{
			return SDimension2(*this) /= n;
		}
	}
}