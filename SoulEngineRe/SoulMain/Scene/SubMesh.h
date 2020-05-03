#pragma once
#include "../SoulPCH.h"
#include "PrimitiveTypes.h"
#include "../RenderSystem/RenderParameter.h"
#include "../RenderSystem/GPUBufferManager.h"

namespace Soul
{
	enum class SIMPLE_GAMEOBJECT
	{
		SG_CUBE,
		SG_SPHERE,
		SG_PLANE,
		SG_LINE3D,
		SG_POINT3D
	};
	struct Material
	{
		Core::SVector4 ambient;
		Core::SVector4 diffuse;
		Core::SVector4 specular; // w = 镜面反射强度
		Core::SVector4 reflect;
	};
	class SubMesh
	{
	public:
		SubMesh(const std::string& name);

		virtual ~SubMesh();

		RenderParameter* GetRenderParameter()
		{
			return mRenderParameter;
		}

		// 顶点缓存
		void InitializeBuffer();

		template<typename VertexType>
		void CreateBuffer()
		{
			VertexType* vertices = new VertexType[mRenderParameter->mVertexCount];
			//填充顶点
			for (size_t i = 0; i < (*mOringinMeshData).Vertices.size(); ++i)
			{
				if (VertexType::hasPos)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::PosStartByte, sizeof(Core::SVector3),
						&((*mOringinMeshData).Vertices[i].Position), sizeof(Core::SVector3));
				}
				if (VertexType::hasTex)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::TexStartByte, sizeof(Core::SVector2),
						&((*mOringinMeshData).Vertices[i].TexCoord), sizeof(Core::SVector2));
					;
				}
				if (VertexType::hasNor)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::NorStartByte, sizeof(Core::SVector3),
						&((*mOringinMeshData).Vertices[i].Normal), sizeof(Core::SVector3));
				}
				if (VertexType::hasCol)
				{
					memcpy_s(reinterpret_cast<char*>(&vertices[i]) + VertexType::ColStartByte, sizeof(Core::SVector4),
						&((*mOringinMeshData).Vertices[i].Color), sizeof(Core::SVector4));
				}
			}
			mRenderParameter->mSizePerVertex = sizeof(VertexType);
			//创建顶点缓存
			mRenderParameter->mVertexBuffer =
				GPUBufferManager::GetInstance().
				CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_VERTEX,
					vertices,
					sizeof(VertexType) * mRenderParameter->mVertexCount);
			//创建索引缓存
			mRenderParameter->mIndexBuffer =
				GPUBufferManager::GetInstance().
				CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_INDEX,
					&(*mOringinMeshData).Indices[0],
					sizeof(unsigned int) * mRenderParameter->mIndicesCount);
			delete[] vertices;
			vertices = nullptr;
		}

		// 纹理
		void PushTexture(ITexture* texture)
		{
			mTextures.push_back(texture);
		}

		std::vector<ITexture*>& GetTextures()
		{
			return mTextures;
		}

		void ClearTexture()
		{
			mTextures.clear();
		}

		// Shader
		void SetShader(Shader* shader)
		{
			mShader = shader;
		}

		Shader* GetShader()
		{
			return mShader;
		}

		// 材质
		Material* GetMaterial() const
		{
			return mMaterial;
		}

		void SetMaterial(Material* newMaterial)
		{
			if (mMaterial)
			{
				delete mMaterial;
				mMaterial = nullptr;
			}
			mMaterial = newMaterial;;
		}

		// Mesh
		MeshData* GetOriginalMeshDataPtr()
		{
			return mOringinMeshData;
		}

		// 深度模板
		void SetDepthStencil(const DepthStencilType& dst)
		{
			mUseDepthStencil = true;
			mDepthStencilType = dst;
		}

		bool UseDepthStencil()
		{
			return mUseDepthStencil;
		}

		const DepthStencilType& GetDepthStencilType()
		{
			return mDepthStencilType;
		}

		// 光栅化
		void SetRasterizer(const RasterizerType& dst)
		{
			mUseRasterizer = true;
			mRasterizerType = dst;
		}

		bool UseRasterizer()
		{
			return mUseRasterizer;
		}

		const RasterizerType& GetRasterizerType()
		{
			return mRasterizerType;
		}

		// 混合
		void SetBlend(const BlendType& dst)
		{
			mUseBlend = true;
			mBlendType = dst;
		}

		bool UseBlend()
		{
			return mUseBlend;
		}

		const BlendType& GetBlendType()
		{
			return mBlendType;
		}

		void SetParent(GameObject* gameObject)
		{
			mParent = gameObject;
		}

		GameObject* GetParent()
		{
			return mParent;
		}

	protected:
		std::string mName;
		// 是否自定义混合
		bool mUseBlend;
		BlendType mBlendType;

		// 是否自定义光栅化
		bool mUseRasterizer;
		RasterizerType mRasterizerType;

		// 是否自定义深度模板
		bool mUseDepthStencil;
		DepthStencilType mDepthStencilType;

		// 需要渲染的顶点缓存
		RenderParameter* mRenderParameter;
		// Texture 可能会用到多张纹理
		std::vector<ITexture*> mTextures;
		// Bind Shader
		Shader* mShader;
		// 材质
		Material* mMaterial;
		// 原始顶点数据
		MeshData* mOringinMeshData;
		// 归属游戏物体
		GameObject* mParent;
	};
}