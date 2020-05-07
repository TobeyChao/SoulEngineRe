#pragma once
#include "../Core/Mathematics/SVector3.h"
#include "../Core/Mathematics/SMatrix4x4.h"

namespace Soul
{
	class BoundingBox
	{
	public:
		BoundingBox() noexcept
			:
			mCenter(0.0f, 0.0f, 0.0f),
			mLengthToSides(1.f, 1.f, 1.f)
		{}
		void Transform(BoundingBox& Out, const Core::SMatrix4x4& M) const;
		bool Intersects(const BoundingBox& box) const;
		static void CreateFromPoints(BoundingBox& boundingBox, const Core::SVector3& min, const Core::SVector3& max);
		static const size_t CORNER_COUNT = 8;
		Core::SVector3 mCenter;
		Core::SVector3 mLengthToSides;
	};
}