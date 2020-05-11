#include "SubMesh.h"
#include "../RenderSystem/ShaderManager.h"

namespace Soul
{
	SubMesh::SubMesh(const std::string& name)
		:
		mName(name),
		mEnableShadow(false),
		mShader(nullptr),
		mMaterial(nullptr),
		mUseBlend(false),
		mBlendType(BlendType::BT_DEFAULT),
		mUseRasterizer(false),
		mRasterizerType(RasterizerType::RT_DEFAULT),
		mUseDepthStencil(false),
		mDepthStencilType(DepthStencilType::DST_DEFAULT),
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

		if (config["input_layout"] == "pos")
		{
			CreateBuffer<PositionVertex>(GPU_BUFFER_TYPE::GBT_VERTEX);
		}
		else if (config["input_layout"] == "pos_tex")
		{
			CreateBuffer<TextureVertex>(GPU_BUFFER_TYPE::GBT_VERTEX);
		}
		else if (config["input_layout"] == "pos_col")
		{
			CreateBuffer<ColorVertex>(GPU_BUFFER_TYPE::GBT_VERTEX);
		}
		else if (config["input_layout"] == "pos_tex_normal_col")
		{
			CreateBuffer<PosTexNorColVertex>(GPU_BUFFER_TYPE::GBT_VERTEX);
		}
		else if (config["input_layout"] == "pos_tex_col")
		{
			// error
		}
		else
		{

		}
	}
}