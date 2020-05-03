#pragma once
#include <unordered_map>
#include "../Scene/PrimitiveTypes.h"
#include "../Scene/SubMesh.h"
namespace Soul
{
	class ObjLoader
	{
	public:
		struct ObjMaterial
		{
			std::wstring name;
			std::wstring texName;
			bool transparent;
			Core::SVector4 ambient;
			Core::SVector4 diffuse;
			Core::SVector4 specular; // w = 镜面反射强度
			Core::SVector4 reflect;
		};
		enum
		{
			LEFT_HAND_SYSTEM = 1,
			RIGHT_HAND_SYSTEM
		};
	public:
		bool LoadObjModel(std::wstring objFileName, int nHandSystem = RIGHT_HAND_SYSTEM);
		std::vector<SubMesh*>& GetSubMeshes()
		{
			return mSubMesh;
		}
		void Clean();
	private:
		bool LoadObjFile(std::wstring objFileName, int nHandSystem = RIGHT_HAND_SYSTEM);
		bool ParseVertices(std::wstring objFileName, int nHandSystem);
		bool ParseMaterialFile(std::wstring matFileName);
		bool ParseSubset(std::wstring objFileName, int nHandSystem);
		void AddVertex(SubMesh* subMesh, const Vertex& vertex, size_t vpi, size_t vti, size_t vni);

	private:
		std::wstring mParentPath;

		std::vector<Core::SVector3> mVertices;
		std::vector<Core::SVector2> mTextures;
		std::vector<Core::SVector3> mNormals;
		std::vector<ObjMaterial> mMaterial;
		std::unordered_map<std::wstring, unsigned> mVertexCache;

		std::vector<SubMesh*> mSubMesh;
	};
}