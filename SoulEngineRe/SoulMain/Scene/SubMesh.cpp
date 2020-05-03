#include "SubMesh.h"
#include "../RenderSystem/ShaderManager.h"

namespace Soul
{
	SubMesh::SubMesh(const std::string& name)
		:
		mName(name),
		mShader(nullptr),
		mMaterial(nullptr),
		mUseBlend(false),
		mUseRasterizer(false),
		mUseDepthStencil(false),
		mParent(nullptr)
	{
		mOringinMeshData = new MeshData();
		mRenderParameter = new RenderParameter();
		// Ä¬ÈÏ²ÄÖÊ
		mMaterial = new Material();
		mMaterial->ambient = { 0.7250f, 0.7100f, 0.6800f, 1.0f };
		mMaterial->diffuse = { 0.7250f, 0.7100f, 0.6800f, 1.0f };
		mMaterial->specular = { 0.5f, 0.5f, 0.5f, 5.0f };
	}

	SubMesh::~SubMesh()
	{
		if (mOringinMeshData)
		{
			delete mOringinMeshData;
			mOringinMeshData = nullptr;
		}
		if (mMaterial)
		{
			delete mMaterial;
			mMaterial = nullptr;
		}
	}
	void SubMesh::InitializeBuffer()
	{
		if (!mShader)
		{
			return;
		}
		mRenderParameter->mVertexCount = (unsigned int)(*mOringinMeshData).Vertices.size();
		mRenderParameter->mIndicesCount = (unsigned int)(*mOringinMeshData).Indices.size();

		const json& config = mShader->GetShaderConfig();

		if (config["input_layout"] == "pos_tex")
		{
			CreateBuffer<TextureVertex>();
		}
		else if (config["input_layout"] == "pos_col")
		{
			CreateBuffer<ColorVertex>();
		}
		else if (config["input_layout"] == "pos_tex_normal_col")
		{
			CreateBuffer<PosTexNorColVertex>();
		}
		else
		{
			// error
		}
	}
}