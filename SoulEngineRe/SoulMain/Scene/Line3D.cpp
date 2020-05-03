#include "Line3D.h"
#include "../RenderSystem/ShaderManager.h"
#include "../RenderSystem/RenderParameter.h"
#include "../RenderSystem/GPUBufferManager.h"
#include "PrimitiveTypes.h"

namespace Soul
{
	Line3D::Line3D(std::string name, const Core::SVector3& start, const Core::SVector3& end)
		:
		SubMesh(name),
		mStart(start),
		mEnd(end)
	{
		mRenderParameter->mPrimitiveTopology = PrimitiveTopology::PT_LINELIST;
		//设置Shader
		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Color");
		SetShader(shader);
	}
	void Line3D::SetShader(Shader* shader)
	{
		SubMesh::SetShader(shader);
		mRenderParameter->mVertexCount = 2u;
		mRenderParameter->mIndicesCount = 2u;
		//定义顶点
		ColorVertex vertices[2];
		vertices[0] = ColorVertex(mStart.x, mStart.y, mStart.z, 1.0, 1.0, 1.0, 1.0);
		vertices[1] = ColorVertex(mEnd.x, mEnd.y, mEnd.z, 1.0, 1.0, 1.0, 1.0);
		//定义索引
		unsigned int indices[] = { 0, 1 };
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
				&indices[0],
				sizeof(unsigned int) * mRenderParameter->mIndicesCount);
	}
}