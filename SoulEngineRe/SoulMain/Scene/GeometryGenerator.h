#pragma once
#include <functional>
#include "PrimitiveTypes.h"

namespace Soul
{
	class GeometryGenerator
	{
	public:
		// Creates a box centered at the origin with the given dimensions.
		void CreateBox(float width, float height, float depth, MeshData& meshData);

		// Creates a sphere centered at the origin with the given radius.  The
		// slices and stacks parameters control the degree of tessellation.
		void CreateSphere(float radius, unsigned sliceCount, unsigned stackCount, MeshData& meshData);

		// Creates a cylinder parallel to the y-axis, and centered about the origin.  
		// The bottom and top radius can vary to form various cone shapes rather than true
		// cylinders.  The slices and stacks parameters control the degree of tessellation.
		void CreateCylinder(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, MeshData& meshData);

		// Creates an mxn grid in the xz-plane with m rows and n columns, centered
		// at the origin with the specified width and depth.
		void CreateGrid(float width, float depth, unsigned m, unsigned n, const Core::SVector2& maxTexCoord, MeshData& meshData);

		// Creates an mxn terrain in the xz-plane with m rows and n columns, centered
		// at the origin with the specified width and depth.
		void CreateTerrain(float width, float depth, unsigned m, unsigned n, MeshData& meshData,
			const std::function<float(int, int)>& heightFunc = [](int x, int z) { return 0.0f; },
			const std::function<Core::SVector4(int, int)>& colorFunc = [](int x, int z) { return Core::SVector4(1.0f, 1.0f, 1.0f, 1.0f); }
		);

		// Creates a line
		void CreateLine3D(const Core::SVector3& start, const Core::SVector3& end, MeshData& meshData);

		// Creates a point
		void CreatePoint3D(const Core::SVector3& pos, MeshData& meshData);

		// Creates a quad covering the screen in NDC coordinates.  This is useful for
		// postprocessing effects.
		void CreateFullscreenQuad(MeshData& meshData);

	private:
		void Subdivide(MeshData& meshData);
		void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, MeshData& meshData);
		void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, MeshData& meshData);
		void ComputeNormal(const Core::SVector3& posA, const Core::SVector3& posB, const Core::SVector3& posC, Core::SVector3& outNormal);
	};
}