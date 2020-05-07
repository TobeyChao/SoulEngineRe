#include "BoundingBox.h"
#include <algorithm>

namespace Soul
{
	void BoundingBox::Transform(BoundingBox& Out, const Core::SMatrix4x4& M) const
	{
		static Core::SVector3 BoxOffset[8] =
		{
			{ -1.0f, -1.0f,  1.0f },
			{  1.0f, -1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{ -1.0f,  1.0f, -1.0f },
		};

		// Load center and extents.
		Core::SVector3 vCenter = mCenter;
		Core::SVector3 vExtents = mLengthToSides;

		// Compute and transform the corners and find new min/max bounds.
		Core::SVector3 Corner = Core::Multiply(vExtents, BoxOffset[0]) + vCenter;
		Corner = Corner * M;

		Core::SVector3 Min, Max;
		Min = Max = Corner;

		for (size_t i = 1; i < CORNER_COUNT; ++i)
		{
			Corner = Core::Multiply(vExtents, BoxOffset[i]) + vCenter;
			Corner = Corner * M;

			Min = { std::min(Min.x, Corner.x),std::min(Min.y, Corner.y),std::min(Min.z, Corner.z) };
			Max = { std::max(Max.x, Corner.x),std::max(Max.y, Corner.y),std::max(Max.z, Corner.z) };
		}

		// Store center and extents.
		Out.mCenter = (Min + Max) * 0.5f;
		Out.mLengthToSides = (Max - Min) * 0.5f;
	}
	bool BoundingBox::Intersects(const BoundingBox& box) const
	{
		Core::SVector3 AMin = mCenter - mLengthToSides;
		Core::SVector3 AMax = mCenter + mLengthToSides;
		Core::SVector3 BMin = box.mCenter - box.mLengthToSides;
		Core::SVector3 BMax = box.mCenter + box.mLengthToSides;
		if (AMin.x > BMax.x || BMin.x > AMax.x) return false;
		if (AMin.y > BMax.y || BMin.y > AMax.y) return false;
		if (AMin.z > BMax.z || BMin.z > AMax.z) return false;
		return true;
	}
	void BoundingBox::CreateFromPoints(BoundingBox& boundingBox, const Core::SVector3& inMin, const Core::SVector3& inMax)
	{
		Core::SVector3 min = { std::min(inMin.x, inMax.x),std::min(inMin.y, inMax.y),std::min(inMin.z, inMax.z) };
		Core::SVector3 max = { std::max(inMin.x, inMax.x),std::max(inMin.y, inMax.y),std::max(inMin.z, inMax.z) };
		boundingBox.mCenter = (min + max) * 0.5f;
		boundingBox.mLengthToSides = (max - min) * 0.5f;
	}
}