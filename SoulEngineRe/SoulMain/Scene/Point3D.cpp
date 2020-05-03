#include "Point3D.h"
#include "../RenderSystem/ShaderManager.h"
#include "../RenderSystem/RenderParameter.h"
#include "../RenderSystem/GPUBufferManager.h"
#include "PrimitiveTypes.h"

namespace Soul
{
	Point3D::Point3D(std::string name)
		:
		SubMesh(name)
	{
		mRenderParameter->mPrimitiveTopology = PrimitiveTopology::PT_POINTLIST;
		//设置Shader
		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Color");
		SetShader(shader);
	}
	void Point3D::SetShader(Shader* shader)
	{
		SubMesh::SetShader(shader);
		
		mRenderParameter->mVertexCount = 1u;
		mRenderParameter->mIndicesCount = 1u;
		ColorVertex vertices[1];
		vertices[0].Position = Core::SVector3(0.0f, 0.0f, 0.0f);
		vertices[0].Color = Core::SVector4(1.0f, 1.0f, 1.0f, 1.0f);
		unsigned int index[1];
		index[0] = 0u;

		mRenderParameter->mSizePerVertex = sizeof(ColorVertex);
		//创建顶点缓存
		mRenderParameter->mVertexBuffer =
			GPUBufferManager::GetInstance().
			CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_VERTEX,
				vertices,
				sizeof(ColorVertex) * mRenderParameter->mVertexCount);
		//创建索引缓存
		mRenderParameter->mIndexBuffer =
			GPUBufferManager::GetInstance().
			CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_INDEX,
				&index[0],
				sizeof(unsigned int) * mRenderParameter->mIndicesCount);
	}
}