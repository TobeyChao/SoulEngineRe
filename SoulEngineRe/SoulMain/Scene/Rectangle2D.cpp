#include "Rectangle2D.h"
#include "../RenderSystem/ShaderManager.h"
#include "../RenderSystem/RenderParameter.h"
#include "../RenderSystem/GPUBufferManager.h"
#include "PrimitiveTypes.h"

namespace Soul
{
	Rectangle2D::Rectangle2D(std::string name, const Core::SDimension2& size)
		:
		SubMesh(name),
		mSize(size)
	{
		//设置Shader
		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Color");
		SetShader(shader);
	}

	void Rectangle2D::SetShader(Shader* shader)
	{
		SubMesh::SetShader(shader);
		mRenderParameter->mVertexCount = 4u;
		mRenderParameter->mIndicesCount = 6u;
		//定义顶点
		ColorTextureVertex* vertices = new ColorTextureVertex[4u];
		vertices[0] = ColorTextureVertex(-(float)mSize.width / 2.0f, -(float)mSize.height / 2.0f, -1.0f, 0.0f, 1.0f, 1.0, 1.0, 1.0, 1.0f);
		vertices[1] = ColorTextureVertex(-(float)mSize.width / 2.0f, (float)mSize.height / 2.0f, -1.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0, 1.0f);
		vertices[2] = ColorTextureVertex((float)mSize.width / 2.0f, (float)mSize.height / 2.0f, -1.0f, 1.0f, 0.0f, 1.0, 1.0, 1.0, 1.0f);
		vertices[3] = ColorTextureVertex((float)mSize.width / 2.0f, -(float)mSize.height / 2.0f, -1.0f, 1.0f, 1.0f, 1.0, 1.0, 1.0, 1.0f);
		//定义索引
		unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
		mRenderParameter->mSizePerVertex = sizeof(ColorTextureVertex);
		//创建顶点缓存
		mRenderParameter->mVertexBuffer =
			GPUBufferManager::GetInstance().
			CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_VERTEX,
				vertices,
				sizeof(ColorTextureVertex) * mRenderParameter->mVertexCount);
		//创建索引缓存
		mRenderParameter->mIndexBuffer =
			GPUBufferManager::GetInstance().
			CreateGPUBuffer(GPU_BUFFER_TYPE::GBT_INDEX,
				&indices[0],
				sizeof(unsigned int) * mRenderParameter->mIndicesCount);
	}

	//void SRectangle2D::Render(const Scene::ISceneNode * sceneNode)
	//{
	//	//渲染管线-1-输入装配阶段
	//	UINT stride = sizeof(ColorTextureVertex);
	//	UINT offset = 0u;
	//	ID3D11DeviceContext *deviceContext =
	//		((Graphics::CD3D11Driver*)sceneNode->GetSceneManager()->GetGraphicsDriver())->GetD3D11DeviceContext();
	//	Scene::ISceneNodeCamera* camera = sceneNode->GetSceneManager()->GetActiveCamera();
	//	//设置顶点缓冲
	//	deviceContext->IASetVertexBuffers(0u, 1u, &mVertBuffer, &stride, &offset);
	//	//设置索引缓冲
	//	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0u);
	//	//设置基元拓扑
	//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	//关闭z缓冲区
	//	((Graphics::CD3D11Driver*)sceneNode->GetSceneManager()->GetGraphicsDriver())->TurnOffZBuffer();
	//	//渲染scenenode
	//	mShader->RenderNode(mIndicesCount, sceneNode->GetAbsoluteTransformation(), camera->GetViewMatrix(true), camera->GetProjectionMatrix(true), mMaterial);
	//	//开启z缓冲区
	//	((Graphics::CD3D11Driver*)sceneNode->GetSceneManager()->GetGraphicsDriver())->TurnOnZBuffer();
	//}
}
