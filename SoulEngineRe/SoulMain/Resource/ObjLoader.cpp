#include "ObjLoader.h"
#include "TextureManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#undef min
#undef max

namespace Soul
{
	const std::wstring newExtension = L"cm";
	using namespace std::filesystem;
	bool ObjLoader::LoadObjModel(const std::wstring& objFileName, int nHandSystem)
	{
		path pathObj(objFileName);
		std::wstring customModelFileName = pathObj.replace_extension(newExtension).wstring();
		if (CheckIfFile(customModelFileName))
		{
			return LoadCustomModelFile(customModelFileName);
		}
		return LoadObjFile(objFileName, nHandSystem);
	}

	void ObjLoader::Clean()
	{
		mVertexCache.clear();
		mVertices.clear();
		mTextures.clear();
		mNormals.clear();
		mMaterial.clear();
		mSubMesh.clear();
	}

	bool ObjLoader::LoadObjFile(const std::wstring& objFileName, int nHandSystem)
	{
		if (!CheckIfFile(objFileName))
		{
			return false;
		}
		path pathObj(objFileName);
		mParentPath = pathObj.parent_path().wstring();
		mMin = { 3.402823466e+38F, 3.402823466e+38F, 3.402823466e+38F };
		mMax = { -3.402823466e+38F, -3.402823466e+38F, -3.402823466e+38F };
		ParseVertices(objFileName, nHandSystem);
		ParseSubset(objFileName, nHandSystem);
		if (!mSubMesh.empty())
		{
			mSubMesh[0]->GetOriginalMeshDataPtr()->Min = mMin;
			mSubMesh[0]->GetOriginalMeshDataPtr()->Max = mMax;
		}
		// 生成二进制文件
		GenerateCustomModelFile(pathObj.replace_extension(newExtension));
		return true;
	}

	bool ObjLoader::GenerateCustomModelFile(const std::wstring& customModelFileName)
	{
		// [Part数目] sizeof(size_t)字节
		// [AABB盒顶点vMax] 12字节
		// [AABB盒顶点vMin] 12字节
		// [Part
		//   [环境光材质文件名]520字节
		//   [漫射光材质文件名]520字节
		//   [材质]sizeof(Material)字节
		//   [顶点数]sizeof(size_t)字节
		//   [索引数]sizeof(size_t)字节
		//   [顶点]sizeof(Vertex)*顶点数 字节
		//   [索引]unsigned*索引数 字节
		// ]
		// ...
		std::ofstream fout(customModelFileName, std::ios::out | std::ios::binary);
		size_t parts = mSubMesh.size();
		// [Part数目] 4字节
		fout.write(reinterpret_cast<const char*>(&parts), sizeof(size_t));
		// [AABB盒顶点vMax] 12字节
		fout.write(reinterpret_cast<const char*>(&mMax), sizeof(Core::SVector3));
		// [AABB盒顶点vMin] 12字节
		fout.write(reinterpret_cast<const char*>(&mMin), sizeof(Core::SVector3));
		// [Part
		for (size_t i = 0; i < parts; ++i)
		{
			wchar_t filePath[MAX_PATH] = L"";
			std::wstring mapDiffuse;
			if (mSubMesh[i]->GetTextures().size() > 0)
			{
				mapDiffuse = mSubMesh[i]->GetTextures()[0]->GetName();
			}
			wcscpy_s(filePath, mapDiffuse.c_str());
			// [漫射光材质文件名]520字节
			fout.write(reinterpret_cast<const char*>(filePath), MAX_PATH * sizeof(wchar_t));
			// [材质]64字节
			fout.write(reinterpret_cast<const char*>(mSubMesh[i]->GetMaterial()), sizeof(Material));
			size_t vertexCount = mSubMesh[i]->GetOriginalMeshDataPtr()->Vertices.size();
			// [顶点数]4字节
			fout.write(reinterpret_cast<const char*>(&vertexCount), sizeof(size_t));
			size_t indexCount = (UINT)mSubMesh[i]->GetOriginalMeshDataPtr()->Indices.size();
			// [索引数]4字节
			fout.write(reinterpret_cast<const char*>(&indexCount), sizeof(size_t));
			// [顶点]32*顶点数 字节
			fout.write(reinterpret_cast<const char*>(mSubMesh[i]->GetOriginalMeshDataPtr()->Vertices.data()), vertexCount * sizeof(Vertex));
			// [索引]4*索引数 字节
			fout.write(reinterpret_cast<const char*>(mSubMesh[i]->GetOriginalMeshDataPtr()->Indices.data()), indexCount * sizeof(unsigned));
		}
		// ]
		fout.close();
		return true;
	}

	bool ObjLoader::LoadCustomModelFile(const std::wstring& customModelFileName)
	{
		// [Part数目] sizeof(size_t)字节
		// [AABB盒顶点vMax] 12字节
		// [AABB盒顶点vMin] 12字节
		// [Part
		//   [漫射光材质文件名]520字节
		//   [材质]sizeof(Material)字节
		//   [顶点数]sizeof(size_t)字节
		//   [索引数]sizeof(size_t)字节
		//   [顶点]sizeof(Vertex)*顶点数 字节
		//   [索引]unsigned*索引数 字节
		// ]
		// ...
		std::ifstream fin(customModelFileName, std::ios::in | std::ios::binary);
		if (!fin.is_open())
			return false;

		size_t parts = 0;
		// [Part数目] 4字节
		fin.read(reinterpret_cast<char*>(&parts), sizeof(size_t));
		mSubMesh.resize(parts);
		for (size_t i = 0; i < parts; i++)
		{
			mSubMesh[i] = new SubMesh("");
		}
		// [AABB盒顶点vMax] 12字节
		fin.read(reinterpret_cast<char*>(&mMax), sizeof(Core::SVector3));
		// [AABB盒顶点vMin] 12字节
		fin.read(reinterpret_cast<char*>(&mMin), sizeof(Core::SVector3));
		mSubMesh[0]->GetOriginalMeshDataPtr()->Max = mMax;
		mSubMesh[0]->GetOriginalMeshDataPtr()->Min = mMin;
		for (size_t i = 0; i < parts; ++i)
		{
			wchar_t filePath[MAX_PATH];
			// [漫射光材质文件名]
			fin.read(reinterpret_cast<char*>(filePath), MAX_PATH * sizeof(wchar_t));
			std::wstring textureFilePath(filePath);
			if (!textureFilePath.empty())
			{
				mSubMesh[i]->PushTexture(TextureManager::GetInstance().GetTexture(textureFilePath));
			}
			// [材质]64字节
			Material* mat = new Material();
			fin.read(reinterpret_cast<char*>(mat), sizeof(Material));
			mSubMesh[i]->SetMaterial(mat);
			size_t vertexCount, indexCount;
			// [顶点数]4字节
			fin.read(reinterpret_cast<char*>(&vertexCount), sizeof(size_t));
			// [索引数]4字节
			fin.read(reinterpret_cast<char*>(&indexCount), sizeof(size_t));
			// [顶点]32*顶点数 字节
			mSubMesh[i]->GetOriginalMeshDataPtr()->Vertices.resize(vertexCount);
			fin.read(reinterpret_cast<char*>(mSubMesh[i]->GetOriginalMeshDataPtr()->Vertices.data()), vertexCount * sizeof(Vertex));
			// [索引]4*索引数 字节
			mSubMesh[i]->GetOriginalMeshDataPtr()->Indices.resize(indexCount);
			fin.read(reinterpret_cast<char*>(mSubMesh[i]->GetOriginalMeshDataPtr()->Indices.data()), indexCount * sizeof(unsigned));
		}

		fin.close();
		return true;
	}

	bool ObjLoader::ParseVertices(const std::wstring& objFileName, int nHandSystem)
	{
		std::wifstream in(objFileName);
		if (!in)
		{
			return false;
		}
		std::wstring line, word;
		while (!in.eof())
		{
			std::getline(in, line);
			std::wistringstream record(line);
			record >> word;
			if (word == L"mtllib")
			{
				std::wstring MaterialFileName;
				record >> word;
				MaterialFileName = mParentPath + L"/" + word;
				ParseMaterialFile(MaterialFileName);
			}
			else if (word == L"v")//v:pos
			{
				record >> word;
				float PosX = std::stof(word);
				record >> word;
				float PosY = std::stof(word);
				record >> word;
				float PosZ = std::stof(word);
				if (RIGHT_HAND_SYSTEM == nHandSystem)
					PosZ = -PosZ;
				mMin = { std::min(PosX, mMin.x), std::min(PosY, mMin.y), std::min(PosZ, mMin.z) };
				mMax = { std::max(PosX, mMax.x), std::max(PosY, mMax.y), std::max(PosZ, mMax.z) };
				mVertices.push_back(Core::SVector3(PosX, PosY, PosZ));
			}
			else if (word == L"vn")//vn:normal
			{
				record >> word;
				float NormalX = std::stof(word);
				record >> word;
				float NormalY = std::stof(word);
				record >> word;
				float NormalZ = std::stof(word);
				if (RIGHT_HAND_SYSTEM == nHandSystem)
					NormalZ = -NormalZ;
				mNormals.push_back(Core::SVector3(NormalX, NormalY, NormalZ));
			}
			else if (word == L"vt")//vt:tex
			{
				record >> word;
				float u = std::stof(word);
				record >> word;
				float v = std::stof(word);
				if (RIGHT_HAND_SYSTEM == nHandSystem)
					v = 1.0f - v;
				mTextures.push_back(Core::SVector2(u, v));
			}
			else
			{
				;
			}

		}
		return true;
	}

	bool ObjLoader::ParseMaterialFile(const std::wstring& matFileName)
	{
		ObjMaterial* pMat = nullptr;

		std::wifstream in(matFileName);
		if (!in)
		{
			return false;
		}
		std::wstring line, word;
		while (!in.eof())
		{
			std::getline(in, line);
			std::wistringstream record(line);
			record >> word;
			if (word == L"newmtl")
			{
				if (pMat != nullptr)
				{
					mMaterial.push_back(*pMat);
					delete pMat;
				}
				pMat = new ObjMaterial();
				record >> pMat->name;
			}
			else if (pMat != nullptr)
			{
				if (word == L"Ns")
				{
					std::wistringstream re(line);
					re >> word;
					re >> word;
					float SpecularPower = std::stof(word);
					pMat->specular.w = SpecularPower;
				}
				else if (word == L"Ka")
				{
					std::wistringstream re(line);
					re >> word;
					re >> word;
					float r = std::stof(word);
					re >> word;
					float g = std::stof(word);
					re >> word;
					float b = std::stof(word);
					pMat->ambient = Core::SVector4(r, g, b, 1.0f);
				}
				else if (word == L"Kd")
				{
					std::wistringstream re(line);
					re >> word;
					re >> word;
					float r = std::stof(word);
					re >> word;
					float g = std::stof(word);
					re >> word;
					float b = std::stof(word);
					pMat->diffuse = Core::SVector4(r, g, b, 1.0f);
				}
				else if (word == L"Ks")
				{
					std::wistringstream re(line);
					re >> word;
					re >> word;
					float r = std::stof(word);
					re >> word;
					float g = std::stof(word);
					re >> word;
					float b = std::stof(word);
					pMat->specular.x = r;
					pMat->specular.y = g;
					pMat->specular.z = b;
				}
				else if (word == L"Tr")
				{
					std::wistringstream re(line);
					re >> word;
					re >> word;
					float a = std::stof(word);
					if (a > 0)
					{
						pMat->transparent = true;
					}
				}
				else if (word == L"map_Kd")
				{
					std::wistringstream re(line);
					re >> word;
					re >> word;
					pMat->texName = mParentPath + L"\\" + word;
				}
				else
				{
					continue;
				}
			}

		}
		if (pMat != nullptr)
		{
			mMaterial.push_back(*pMat);
			delete pMat;
		}
		return true;
	}

	bool ObjLoader::ParseSubset(const std::wstring& objFileName, int nHandSystem)
	{
		std::wifstream in;
		in.open(objFileName);
		if (!in)
		{
			return false;
		}
		std::wstring line, word;
		while (!in.eof())
		{
			in >> word;
			if (word == L"usemtl")
			{
				mSubMesh.push_back(new SubMesh(""));

				std::wstring materialName;
				in >> materialName;

				auto iter = mMaterial.begin();
				while (iter != mMaterial.end())
				{
					if (iter->name == materialName)
					{
						break;
					}
					iter++;
				}
				if (iter == mMaterial.end())
				{
					return false;
				}
				else
				{
					Material* mat = new Material();
					mat->ambient = iter->ambient;
					mat->diffuse = iter->diffuse;
					mat->specular = iter->specular;
					mSubMesh.back()->SetMaterial(mat);
					if (!iter->texName.empty())
					{
						mSubMesh.back()->PushTexture(TextureManager::GetInstance().GetTexture(iter->texName));
					}
				}
			}
			else if (!mSubMesh.empty())
			{

				if (word == L"f")
				{
					Vertex vertex[3];
					size_t posIndex[3];
					size_t texIndex[3];
					size_t normalIndex[3];

					wchar_t ch;

					if (LEFT_HAND_SYSTEM == nHandSystem)
					{
						in >> posIndex[0] >> ch >> texIndex[0] >> ch >> normalIndex[0]
							>> posIndex[1] >> ch >> texIndex[1] >> ch >> normalIndex[1]
							>> posIndex[2] >> ch >> texIndex[2] >> ch >> normalIndex[2];
					}
					else
					{
						in >> posIndex[2] >> ch >> texIndex[2] >> ch >> normalIndex[2]
							>> posIndex[1] >> ch >> texIndex[1] >> ch >> normalIndex[1]
							>> posIndex[0] >> ch >> texIndex[0] >> ch >> normalIndex[0];
					}

					for (int i = 0; i < 3; i++)
					{
						vertex[i].Position = mVertices[posIndex[i] - (size_t)1];
						vertex[i].TexCoord = mTextures[texIndex[i] - (size_t)1];
						vertex[i].Normal = mNormals[normalIndex[i] - (size_t)1];
						AddVertex(mSubMesh.back(), vertex[i], posIndex[i], texIndex[i], normalIndex[i]);
					}
				}
			}
			else
			{
				;
			}
		}
		return true;
	}

	void ObjLoader::AddVertex(SubMesh* subMesh, const Vertex& vertex, size_t vpi, size_t vti, size_t vni)
	{
		std::wstring idxStr = std::to_wstring(vpi) + L"/" + std::to_wstring(vti) + L"/" + std::to_wstring(vni);

		auto it = mVertexCache.find(idxStr);
		// 顶点已经录入Cache
		if (it != mVertexCache.end())
		{
			subMesh->GetOriginalMeshDataPtr()->Indices.push_back(it->second);
		}
		else
		{
			subMesh->GetOriginalMeshDataPtr()->Vertices.push_back(vertex);
			// 录入Cache索引信息
			unsigned pos = (unsigned)subMesh->GetOriginalMeshDataPtr()->Vertices.size() - 1u;
			mVertexCache[idxStr] = pos;
			subMesh->GetOriginalMeshDataPtr()->Indices.push_back(pos);
		}
	}
}