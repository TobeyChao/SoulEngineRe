#pragma once
#include <vector>
#include "../Core/Mathematics/SMatrix4x4.h"
#include "../Core/Mathematics/SVector2.h"
#include "../Core/Mathematics/SVector3.h"
#include "../Core/Mathematics/SVector4.h"

namespace Soul
{
	struct PositionVertex
	{
		PositionVertex()
			:
			Position(0.0f, 0.0f, 0.0f)
		{}
		PositionVertex(float x, float y, float z)
			:
			Position(x, y, z)
		{}
		Core::SVector3 Position;
		static const bool hasPos = true;
		static const bool hasTex = false;
		static const bool hasNor = false;
		static const bool hasCol = false;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = 0;
		static const unsigned NorStartByte = 0;
		static const unsigned ColStartByte = 0;
	};

	struct TextureVertex
	{
		TextureVertex()
			:
			Position(0.0f, 0.0f, 0.0f),
			TexCoord(0.0f, 0.0f)
		{}
		TextureVertex(float x, float y, float z, float u, float v)
			:
			Position(x, y, z),
			TexCoord(u, v)
		{}
		Core::SVector3 Position;
		Core::SVector2 TexCoord;
		static const bool hasPos = true;
		static const bool hasTex = true;
		static const bool hasNor = false;
		static const bool hasCol = false;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = sizeof(Core::SVector3);
		static const unsigned NorStartByte = 0;
		static const unsigned ColStartByte = 0;
	};

	struct ColorVertex
	{
		ColorVertex()
		{}
		ColorVertex(float x, float y, float z, float r, float g, float b, float a)
			:
			Position(x, y, z),
			Color(r, g, b, a)
		{}
		Core::SVector3 Position;
		Core::SVector4 Color;
		static const bool hasPos = true;
		static const bool hasTex = false;
		static const bool hasNor = false;
		static const bool hasCol = true;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = 0;
		static const unsigned NorStartByte = 0;
		static const unsigned ColStartByte = sizeof(Core::SVector3);
	};

	struct ColorTextureVertex
	{
		ColorTextureVertex()
			:
			Position(0.0f, 0.0f, 0.0f),
			TexCoord(0.0f, 0.0f),
			Color(0.0f, 0.0f, 0.0f, 0.0f)
		{}
		ColorTextureVertex(float x, float y, float z, float u, float v,
			float r, float g, float b, float a)
			:
			Position(x, y, z),
			TexCoord(u, v),
			Color(r, g, b, a)
		{}
		Core::SVector3 Position;
		Core::SVector2 TexCoord;
		Core::SVector4 Color;
		static const bool hasPos = true;
		static const bool hasTex = true;
		static const bool hasNor = false;
		static const bool hasCol = true;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = sizeof(Core::SVector3);
		static const unsigned NorStartByte = 0;
		static const unsigned ColStartByte = sizeof(Core::SVector3) + sizeof(Core::SVector2);
	};

	struct PosTexNorVertex
	{
		PosTexNorVertex() {}
		PosTexNorVertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			:
			Position(x, y, z),
			TexCoord(u, v),
			Normal(nx, ny, nz)
		{}
		Core::SVector3 Position;
		Core::SVector2 TexCoord;
		Core::SVector3 Normal;
		static const bool hasPos = true;
		static const bool hasTex = true;
		static const bool hasNor = true;
		static const bool hasCol = false;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = sizeof(Core::SVector3);
		static const unsigned NorStartByte = sizeof(Core::SVector3) + sizeof(Core::SVector2);
		static const unsigned ColStartByte = 0;
	};

	struct PosNorColVertex
	{
		PosNorColVertex() {}
		PosNorColVertex(float x, float y, float z,
			float nx, float ny, float nz,
			float r, float g, float b, float a)
			:
			Position(x, y, z),
			normal(nx, ny, nz),
			color(r, g, b, a)
		{}
		Core::SVector3 Position;
		Core::SVector3 normal;
		Core::SVector4 color;
		static const bool hasPos = true;
		static const bool hasTex = false;
		static const bool hasNor = true;
		static const bool hasCol = true;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = 0;
		static const unsigned NorStartByte = sizeof(Core::SVector3);
		static const unsigned ColStartByte = sizeof(Core::SVector3) + sizeof(Core::SVector3);
	};

	struct PosTexNorColVertex
	{
		PosTexNorColVertex() {}
		PosTexNorColVertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float r, float g, float b, float a)
			:
			Position(x, y, z),
			TexCoord(u, v),
			Normal(nx, ny, nz),
			Color(r, g, b, a)
		{}
		Core::SVector3 Position;
		Core::SVector2 TexCoord;
		Core::SVector3 Normal;
		Core::SVector4 Color;
		static const bool hasPos = true;
		static const bool hasTex = true;
		static const bool hasNor = true;
		static const bool hasCol = true;

		static const unsigned PosStartByte = 0;
		static const unsigned TexStartByte = sizeof(Core::SVector3);
		static const unsigned NorStartByte = sizeof(Core::SVector3) + sizeof(Core::SVector2);
		static const unsigned ColStartByte = sizeof(Core::SVector3) + sizeof(Core::SVector2) + sizeof(Core::SVector3);
	};

	struct Vertex
	{
		Vertex()
		{}

		Vertex(const Core::SVector3& p,
			const Core::SVector3& n,
			const Core::SVector3& t,
			const Core::SVector2& uv,
			const Core::SVector4& col)
			:
			Position(p),
			Normal(n),
			TangentU(t),
			TexCoord(uv),
			Color(col)
		{}

		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v,
			float r, float g, float b, float a)
			:
			Position(px, py, pz),
			Normal(nx, ny, nz),
			TangentU(tx, ty, tz),
			TexCoord(u, v),
			Color(r, g, b, a)
		{}

		Core::SVector3 Position;
		Core::SVector3 Normal;
		Core::SVector3 TangentU;
		Core::SVector2 TexCoord;
		Core::SVector4 Color;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<unsigned> Indices;
		// 最大最小点不一定在几何体面上，而是取x\y\z的最大最小值组合而成
		Core::SVector3 Min;
		Core::SVector3 Max;
	};
}