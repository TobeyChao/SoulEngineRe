#include "GeometryGenerator.h"
#include "../Core/Mathematics/MathConsts.h"

namespace Soul
{
	void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
	{
		//
		// Create the vertices.
		//

		Vertex v[24];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the back face vertex data.
		v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the top face vertex data.
		v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the bottom face vertex data.
		v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the left face vertex data.
		v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the right face vertex data.
		v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		meshData.Vertices.assign(&v[0], &v[24]);

		//
		// Create the indices.
		//

		unsigned i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		meshData.Indices.assign(&i[0], &i[36]);
	}

	void GeometryGenerator::CreateSphere(float radius, unsigned sliceCount, unsigned stackCount, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		meshData.Vertices.push_back(topVertex);

		float phiStep = Core::SM_PI / stackCount;
		float thetaStep = 2.0f * Core::SM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (unsigned i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (unsigned j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position.x = radius * sinf(phi) * cosf(theta);
				v.Position.y = radius * cosf(phi);
				v.Position.z = radius * sinf(phi) * sinf(theta);

				// Partial derivative of P with respect to theta
				v.TangentU.x = -radius * sinf(phi) * sinf(theta);
				v.TangentU.y = 0.0f;
				v.TangentU.z = +radius * sinf(phi) * cosf(theta);

				v.TangentU.Normalize();

				v.Normal = v.Position;
				v.Normal.Normalize();

				v.TexCoord.x = theta / Core::SM_2PI;
				v.TexCoord.y = phi / Core::SM_PI;

				meshData.Vertices.push_back(v);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (unsigned i = 1; i <= sliceCount; ++i)
		{
			meshData.Indices.push_back(0);
			meshData.Indices.push_back(i + 1);
			meshData.Indices.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		unsigned baseIndex = 1;
		unsigned ringVertexCount = sliceCount + 1;
		for (unsigned i = 0; i < stackCount - 2; ++i)
		{
			for (unsigned j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		unsigned southPoleIndex = (unsigned)meshData.Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (unsigned i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(southPoleIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, unsigned sliceCount, unsigned stackCount, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Build Stacks.
		// 

		float stackHeight = height / stackCount;

		// Amount to increment radius as we move up each stack level from bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;

		unsigned ringCount = stackCount + 1;

		// Compute vertices for each stack ring starting at the bottom and moving up.
		for (unsigned i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;

			// vertices of ring
			float dTheta = 2.0f * Core::SM_PI / sliceCount;
			for (unsigned j = 0; j <= sliceCount; ++j)
			{
				Vertex vertex;

				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);

				vertex.Position = Core::SVector3(r * c, y, r * s);

				vertex.TexCoord.x = (float)j / sliceCount;
				vertex.TexCoord.y = 1.0f - (float)i / stackCount;

				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the v tex-coord.
				//   Let r0 be the bottom radius and let r1 be the top radius.
				//   y(v) = h - hv for v in [0,1].
				//   r(v) = r1 + (r0-r1)v
				//
				//   x(t, v) = r(v)*cos(t)
				//   y(t, v) = h - hv
				//   z(t, v) = r(v)*sin(t)
				// 
				//  dx/dt = -r(v)*sin(t)
				//  dy/dt = 0
				//  dz/dt = +r(v)*cos(t)
				//
				//  dx/dv = (r0-r1)*cos(t)
				//  dy/dv = -h
				//  dz/dv = (r0-r1)*sin(t)

				// This is unit length.
				vertex.TangentU = Core::SVector3(-s, 0.0f, c);

				float dr = bottomRadius - topRadius;
				Core::SVector3 bitangent(dr * c, -height, dr * s);

				vertex.Normal = Core::Cross(vertex.TangentU, bitangent);
				vertex.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

				meshData.Vertices.push_back(vertex);
			}
		}
		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		unsigned ringVertexCount = sliceCount + 1;

		// Compute indices for each stack.
		for (unsigned i = 0; i < stackCount; ++i)
		{
			for (unsigned j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(i * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

				meshData.Indices.push_back(i * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
				meshData.Indices.push_back(i * ringVertexCount + j + 1);
			}
		}

		BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
		BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	}

	void GeometryGenerator::CreateGrid(float width, float depth, unsigned m, unsigned n, const Core::SVector2& maxTexCoord, MeshData& meshData)
	{
		unsigned vertexCount = m * n;
		unsigned faceCount = (m - 1) * (n - 1) * 2;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = maxTexCoord.x / (n - 1);
		float dv = maxTexCoord.y / (m - 1);

		meshData.Vertices.resize(vertexCount);
		for (unsigned i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (unsigned j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;

				meshData.Vertices[i * n + j].Position = Core::SVector3(x, 0.0f, z);
				meshData.Vertices[i * n + j].Normal = Core::SVector3(0.0f, 1.0f, 0.0f);
				meshData.Vertices[i * n + j].TangentU = Core::SVector3(1.0f, 0.0f, 0.0f);
				meshData.Vertices[i * n + j].Color = Core::SVector4(1.0f, 1.0f, 1.0f, 1.0f);

				// Stretch texture over grid.
				meshData.Vertices[i * n + j].TexCoord.x = j * du;
				meshData.Vertices[i * n + j].TexCoord.y = i * dv;
			}
		}

		//
		// Create the indices.
		//

		meshData.Indices.resize(faceCount * 3); // 3 indices per face

												// Iterate over each quad and compute indices.
		unsigned k = 0;
		for (unsigned i = 0; i < m - 1; ++i)
		{
			for (unsigned j = 0; j < n - 1; ++j)
			{
				meshData.Indices[k] = i * n + j;
				meshData.Indices[k + 1] = i * n + j + 1;
				meshData.Indices[k + 2] = (i + 1) * n + j;

				meshData.Indices[k + 3] = (i + 1) * n + j;
				meshData.Indices[k + 4] = i * n + j + 1;
				meshData.Indices[k + 5] = (i + 1) * n + j + 1;

				k += 6; // next quad
			}
		}
	}

	void GeometryGenerator::CreateTerrain(float width, float depth, unsigned m, unsigned n, MeshData& meshData,
		const std::function<float(int, int)>& heightFunc,
		const std::function<Core::SVector4(int, int)>& colorFunc)
	{
		size_t vertexCount = (m - 1u) * (n - 1u) * 4u;
		size_t faceCount = (m - 1u) * (n - 1u) * 2u;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		meshData.Vertices.resize(vertexCount);
		meshData.Indices.resize(faceCount * 3u);

		// 面的法向量
		std::vector<Core::SVector3> faceNormals;
		faceNormals.resize(faceCount / 2);

		int index = 0, index1, index2, index3, index4;
		size_t k = 0;
		float z;
		float x;
		for (unsigned i = 0; i < (m - 1); i++)
		{
			for (unsigned j = 0; j < (n - 1); j++)
			{
				// Upper left.
				index1 = index;
				z = halfDepth - i * dz;
				x = -halfWidth + j * dx;
				meshData.Vertices[index].Position = Core::SVector3(x, heightFunc(i, j), z);
				meshData.Vertices[index].TexCoord = { 0.0f, 0.0f };
				meshData.Vertices[index].TangentU = Core::SVector3(1.0f, 0.0f, 0.0f);
				meshData.Vertices[index].Color = colorFunc(i, j);
				index++;

				// Upper right.
				index2 = index;
				z = halfDepth - i * dz;
				x = -halfWidth + (j + 1) * dx;
				meshData.Vertices[index].Position = Core::SVector3(x, heightFunc(i, j + 1), z);
				meshData.Vertices[index].TexCoord = { 1.0f, 0.0f };
				meshData.Vertices[index].TangentU = Core::SVector3(1.0f, 0.0f, 0.0f);
				meshData.Vertices[index].Color = colorFunc(i, j);
				index++;

				// Bottom left.
				index3 = index;
				z = halfDepth - (i + 1) * dz;
				x = -halfWidth + j * dx;
				meshData.Vertices[index].Position = Core::SVector3(x, heightFunc(i + 1, j), z);
				meshData.Vertices[index].TexCoord = { 0.0f, 1.0f };
				meshData.Vertices[index].TangentU = Core::SVector3(1.0f, 0.0f, 0.0f);
				meshData.Vertices[index].Color = colorFunc(i, j);

				index++;

				// Bottom Right.
				index4 = index;
				z = halfDepth - (i + 1) * dz;
				x = -halfWidth + (j + 1) * dx;
				meshData.Vertices[index].Position = Core::SVector3(x, heightFunc(i + 1, j + 1), z);
				meshData.Vertices[index].TexCoord = { 1.0f, 1.0f };
				meshData.Vertices[index].TangentU = Core::SVector3(1.0f, 0.0f, 0.0f);
				meshData.Vertices[index].Color = colorFunc(i, j);
				index++;

				//
				// Create the indices.
				//
				meshData.Indices[k] = (index1);
				meshData.Indices[k + 1u] = (index2);
				meshData.Indices[k + 2u] = (index3);
				meshData.Indices[k + 3u] = (index3);
				meshData.Indices[k + 4u] = (index2);
				meshData.Indices[k + 5u] = (index4);

				k += 6u;	// next quad

				//
				// Compute Face Normal
				//
				ComputeNormal(
					meshData.Vertices[index2].Position,
					meshData.Vertices[index1].Position,
					meshData.Vertices[index3].Position,
					faceNormals[i * (n - 1u) + j]);
			}
		}

		// Normal Averaging
		std::vector<Core::SVector3> normalSum;
		normalSum.resize(m * n);
		unsigned normalSumIndex;
		for (int i = 0; i < m; i++)	//row
		{
			for (int j = 0; j < n; j++)	//col
			{
				normalSumIndex = (i * n) + j;
				// 按左上为基点(0, 0)

				// 左上是否有三角形
				if ((j - 1) >= 0 && (i - 1) >= 0)
				{
					index = ((i - 1) * (n - 1)) + (j - 1);
					normalSum[normalSumIndex] += faceNormals[index];
				}

				// 右上是否有三角形
				if ((j < (n - 1)) && (i - 1) >= 0)
				{
					index = ((i - 1) * (n - 1)) + j;
					normalSum[normalSumIndex] += faceNormals[index];
				}

				// 左下是否有三角形
				if ((j - 1) >= 0 && i < (m - 1))
				{
					index = (i * (n - 1)) + (j - 1);
					normalSum[normalSumIndex] += faceNormals[index];
				}

				// 右下是否有三角形
				if ((j < (n - 1)) && i < (m - 1))
				{
					index = (i * (n - 1)) + j;
					normalSum[normalSumIndex] += faceNormals[index];
				}

				normalSum[normalSumIndex].Normalize();
			}
		}

		index = 0;
		for (unsigned i = 0; i < (m - 1); i++)
		{
			for (unsigned j = 0; j < (n - 1); j++)
			{
				index1 = (n * i) + j;
				index2 = (n * i) + (j + 1);
				index3 = (n * (i + 1)) + j;
				index4 = (n * (i + 1)) + (j + 1);

				meshData.Vertices[index++].Normal = normalSum[index1];
				meshData.Vertices[index++].Normal = normalSum[index2];
				meshData.Vertices[index++].Normal = normalSum[index3];
				meshData.Vertices[index++].Normal = normalSum[index4];
			}
		}
	}

	void GeometryGenerator::CreateLine3D(const Core::SVector3& start, const Core::SVector3& end, MeshData& meshData)
	{
		//定义顶点
		meshData.Vertices.resize(2);
		meshData.Vertices[0].Position = start;
		meshData.Vertices[0].Color = { 1.0, 1.0, 1.0, 1.0 };
		meshData.Vertices[1].Position = end;
		meshData.Vertices[1].Color = { 1.0, 1.0, 1.0, 1.0 };
		//定义索引
		meshData.Indices.resize(2);
		meshData.Indices[0] = 0;
		meshData.Indices[1] = 1;
	}

	void GeometryGenerator::CreatePoint3D(const Core::SVector3& pos, MeshData& meshData)
	{
		//定义顶点
		meshData.Vertices.resize(1);
		meshData.Vertices[0].Position = pos;
		meshData.Vertices[0].Color = { 1.0, 1.0, 1.0, 1.0 };
		//定义索引
		meshData.Indices.resize(1);
		meshData.Indices[0] = 0;
	}

	void GeometryGenerator::CreateFullscreenQuad(MeshData& meshData)
	{
		meshData.Vertices.resize(4);
		meshData.Indices.resize(6);

		// Position coordinates specified in NDC space.
		meshData.Vertices[0] = Vertex(
			-1.0f, -1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);

		meshData.Vertices[1] = Vertex(
			-1.0f, +1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f);

		meshData.Vertices[2] = Vertex(
			+1.0f, +1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f);

		meshData.Vertices[3] = Vertex(
			+1.0f, -1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f);

		meshData.Indices[0] = 0;
		meshData.Indices[1] = 1;
		meshData.Indices[2] = 2;

		meshData.Indices[3] = 0;
		meshData.Indices[4] = 2;
		meshData.Indices[5] = 3;
	}

	void GeometryGenerator::Subdivide(MeshData& meshData)
	{
		// Save a copy of the input geometry.
		MeshData inputCopy = meshData;


		meshData.Vertices.resize(0);
		meshData.Indices.resize(0);

		//       v1
		//       *
		//      / \
				//     /   \
	//  m0*-----*m1
//   / \   / \
	//  /   \ /   \
	// *-----*-----*
// v0    m2     v2

		unsigned numTris = (unsigned)inputCopy.Indices.size() / 3;
		for (unsigned i = 0; i < numTris; ++i)
		{
			Vertex v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
			Vertex v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
			Vertex v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

			//
			// Generate the midpoints.
			//

			Vertex m0, m1, m2;

			// For subdivision, we just care about the position component.  We derive the other
			// vertex components in CreateGeosphere.

			m0.Position = Core::SVector3(
				0.5f * (v0.Position.x + v1.Position.x),
				0.5f * (v0.Position.y + v1.Position.y),
				0.5f * (v0.Position.z + v1.Position.z));

			m1.Position = Core::SVector3(
				0.5f * (v1.Position.x + v2.Position.x),
				0.5f * (v1.Position.y + v2.Position.y),
				0.5f * (v1.Position.z + v2.Position.z));

			m2.Position = Core::SVector3(
				0.5f * (v0.Position.x + v2.Position.x),
				0.5f * (v0.Position.y + v2.Position.y),
				0.5f * (v0.Position.z + v2.Position.z));

			//
			// Add new geometry.
			//

			meshData.Vertices.push_back(v0); // 0
			meshData.Vertices.push_back(v1); // 1
			meshData.Vertices.push_back(v2); // 2
			meshData.Vertices.push_back(m0); // 3
			meshData.Vertices.push_back(m1); // 4
			meshData.Vertices.push_back(m2); // 5

			meshData.Indices.push_back(i * 6 + 0);
			meshData.Indices.push_back(i * 6 + 3);
			meshData.Indices.push_back(i * 6 + 5);

			meshData.Indices.push_back(i * 6 + 3);
			meshData.Indices.push_back(i * 6 + 4);
			meshData.Indices.push_back(i * 6 + 5);

			meshData.Indices.push_back(i * 6 + 5);
			meshData.Indices.push_back(i * 6 + 4);
			meshData.Indices.push_back(i * 6 + 2);

			meshData.Indices.push_back(i * 6 + 3);
			meshData.Indices.push_back(i * 6 + 1);
			meshData.Indices.push_back(i * 6 + 4);
		}
	}

	void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
		unsigned sliceCount, unsigned stackCount, MeshData& meshData)
	{
		unsigned baseIndex = (unsigned)meshData.Vertices.size();

		float y = 0.5f * height;
		float dTheta = 2.0f * Core::SM_PI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (unsigned i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius * cosf(i * dTheta);
			float z = topRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v, 1.0f, 1.0f, 1.0f, 1.0f));
		}

		// Cap center vertex.
		meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f));

		// Index of center vertex.
		unsigned centerIndex = (unsigned)meshData.Vertices.size() - 1;

		for (unsigned i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(centerIndex);
			meshData.Indices.push_back(baseIndex + i + 1);
			meshData.Indices.push_back(baseIndex + i);
		}
	}

	void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
		unsigned sliceCount, unsigned stackCount, MeshData& meshData)
	{
		// 
		// Build bottom cap.
		//

		unsigned baseIndex = (unsigned)meshData.Vertices.size();
		float y = -0.5f * height;

		// vertices of ring
		float dTheta = 2.0f * Core::SM_PI / sliceCount;
		for (unsigned i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius * cosf(i * dTheta);
			float z = bottomRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v, 1.0f, 1.0f, 1.0f, 1.0f));
		}

		// Cap center vertex.
		meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f));

		// Cache the index of center vertex.
		unsigned centerIndex = (unsigned)meshData.Vertices.size() - 1;

		for (unsigned i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(centerIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::ComputeNormal(
		const Core::SVector3& posA,
		const Core::SVector3& posB,
		const Core::SVector3& posC, Core::SVector3& outNormal)
	{
		Core::SVector3 vector1, vector2;
		//Compute normals
		vector1 = posA - posB;
		vector2 = posC - posB;
		outNormal = Core::Cross(vector1, vector2);
		outNormal.Normalize();
	}
}