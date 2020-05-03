#pragma once
#include <vector>
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

		// Creates an mxn grid in the xz-plane with m rows and n columns, centered
		// at the origin with the specified width and depth.
		void CreateGrid(float width, float depth, unsigned m, unsigned n, MeshData& meshData);

		// Creates a quad covering the screen in NDC coordinates.  This is useful for
		// postprocessing effects.
		void CreateFullscreenQuad(MeshData& meshData);

	private:
		void Subdivide(MeshData& meshData);
		void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, MeshData& meshData);
		void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, MeshData& meshData);
	};
}