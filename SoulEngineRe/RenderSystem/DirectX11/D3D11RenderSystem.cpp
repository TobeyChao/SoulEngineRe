#include "D3D11RenderSystem.h"
#include "D3D11GPUBuffer.h"
#include "D3D11ShaderManager.h"
#include "D3D11Texture.h"
#include "D3D11GPUBufferManager.h"
#include "D3D11Shader.h"
#include "D3D11RenderWindow.h"
#include "D3D11TextureManager.h"
#include <algorithm>

namespace Soul
{
	D3D11RenderSystem::D3D11RenderSystem()
		:
		mDriverType(D3D_DRIVER_TYPE_UNKNOWN),
		mFeatureLevel(D3D_FEATURE_LEVEL_11_1),
		mBlendDesc(),
		mBlendDescChanged(false),
		mRasterizerDesc(),
		mRasterizerDescChanged(false),
		mDepthStencilDesc(),
		mDepthStencilDescChanged(false),
		mTextureSettingChanged(false)
	{
		mD3D11Device = new D3D11Device();
	}

	D3D11RenderSystem::~D3D11RenderSystem()
	{
		delete mD3D11Device;
	}
	void D3D11RenderSystem::Initialize(const std::string& initConfig)
	{
		RenderSystem::Initialize(initConfig);
		mBlendDescChanged = true;
		mRasterizerDescChanged = true;
		mDepthStencilDescChanged = true;

		// 混合
		ZeroMemory(&mBlendDesc, sizeof(mBlendDesc));

		// 深度模板
		ZeroMemory(&mDepthStencilDesc, sizeof(mDepthStencilDesc));

		// 创建D3DDevice
		if (FAILED(CreateD3D11Device()))
		{
			// 创建Device失败
		}

		//Rasterizer
		// 光栅化
		ZeroMemory(&mRasterizerDesc, sizeof(mRasterizerDesc));
		mRasterizerDesc.FrontCounterClockwise = false; //顺时针
		mRasterizerDesc.DepthClipEnable = true;
		mRasterizerDesc.MultisampleEnable = true;
		mRasterizerDesc.CullMode = D3D11_CULL_BACK;
		mRasterizerDesc.FillMode = D3D11_FILL_SOLID;
		if (FAILED((*mD3D11Device)->CreateRasterizerState(&mRasterizerDesc, mRasterizer.ReleaseAndGetAddressOf())))
		{
			// 创建RasterizerState失败
		}

		D3D11_SAMPLER_DESC dsd = {};
		dsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		dsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		dsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		dsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		dsd.MaxAnisotropy = ((*mD3D11Device)->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2u;
		dsd.MaxLOD = FLT_MAX;
		dsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		if (FAILED((*mD3D11Device)->CreateSamplerState(&dsd, mSamplerStates.ReleaseAndGetAddressOf())))
		{
			// 创建Sampler失败
		}

		mGpuBufferManager = new D3D11GPUBufferManager(*mD3D11Device);
		// 创建texture管理器
		mTextureManager = new D3D11TextureManager(mD3D11Device);
		// 初始化Shader管理器
		mShaderManager = new D3D11ShaderManager(*mD3D11Device);
		// 设置Shader目录，自动加载所有shader
		mShaderManager->SetJsonConfig(mConfig["shader_manager_config"], true);
		mShaderManager->AddShader(L"Texture2D");
		mShaderManager->AddShader(L"Basic");
		mShaderManager->AddShader(L"Color");
	}
	const std::string& D3D11RenderSystem::GetRenderSystemName() const
	{
		static std::string strName("dx11_render_system");
		return strName;
	}
	RenderWindow* D3D11RenderSystem::CreateRenderWindow(const json& config)
	{
		auto window = new D3D11RenderWindow(*mD3D11Device);
		if (!window)
		{
			return nullptr;
		}
		bool ret =  window->Initialize(config);
		if (!ret)
		{
			return nullptr;
		}
		AddRenderTarget(window);
		return window;
	}
	void D3D11RenderSystem::SetRenderTarget(RenderTarget* renderTarget)
	{
		RenderSystem::SetRenderTarget(renderTarget);
		D3D11RenderWindow* renderWindow = dynamic_cast<D3D11RenderWindow*>(mActiveRenderTarget);
		ID3D11RenderTargetView* renderTargetView = renderWindow->GetRenderTargetView();
		ID3D11DepthStencilView* depthStencilView = renderWindow->GetDepthStencilView();
		mD3D11Device->GetDeviceContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	}
	void D3D11RenderSystem::SetViewport(Viewport* viewport)
	{
		if (viewport)
		{
			mActiveViewport = viewport;
			auto renderTarget = mActiveViewport->GetRenderTarget();
			SetRenderTarget(renderTarget);

			D3D11_VIEWPORT d3dViewport;

			d3dViewport.TopLeftX = static_cast<FLOAT>(viewport->GetViewportLeft());
			d3dViewport.TopLeftY = static_cast<FLOAT>(viewport->GetViewportTop());
			d3dViewport.Width = static_cast<FLOAT>(viewport->GetViewportWidth());
			d3dViewport.Height = static_cast<FLOAT>(viewport->GetViewportHeight());
			d3dViewport.MinDepth = 0.0f;
			d3dViewport.MaxDepth = 1.0f;

			mD3D11Device->GetDeviceContext()->RSSetViewports(1, &d3dViewport);
		}
		else
		{
			mActiveViewport = nullptr;
			SetRenderTarget(nullptr);
		}
	}
	void D3D11RenderSystem::SetTexture(size_t slot, ITexture* texture)
	{
		mTextures[slot] = texture;
		mTextureNum = slot + 1;
		mTextureSettingChanged = true;
	}
	void D3D11RenderSystem::BindShader(Shader* shader)
	{
		mCurrentShader = shader;
	}
	void D3D11RenderSystem::Clear(const Core::SColorf& color)
	{
		if (mActiveRenderTarget)
		{
			D3D11RenderWindow* renderTarget = dynamic_cast<D3D11RenderWindow*>(mActiveRenderTarget);
			mD3D11Device->GetDeviceContext()->ClearRenderTargetView(renderTarget->GetRenderTargetView(), color.Get());
			mD3D11Device->GetDeviceContext()->ClearDepthStencilView(renderTarget->GetDepthStencilView(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
	void D3D11RenderSystem::Render(const RenderParameter& rp)
	{
		if (rp.mVertexCount == 0)
		{
			return;
		}
		// 输入装配阶段
		UINT stride = rp.mSizePerVertex;
		UINT offset = 0u;
		ID3D11DeviceContext* deviceContext = mD3D11Device->GetDeviceContext();
		D3D11GPUBuffer* gpuBuffer = dynamic_cast<D3D11GPUBuffer*>(rp.mVertexBuffer);
		ID3D11Buffer* d3d11VertBuffer = gpuBuffer->GetD3D11Buffer();
		gpuBuffer = dynamic_cast<D3D11GPUBuffer*>(rp.mIndexBuffer);
		ID3D11Buffer* d3d11IndexBuffer = gpuBuffer->GetD3D11Buffer();
		// 
		deviceContext->IASetVertexBuffers(0u, 1u, &d3d11VertBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(d3d11IndexBuffer, DXGI_FORMAT_R32_UINT, 0u);

		switch (rp.mPrimitiveTopology)
		{
		case PrimitiveTopology::PT_POINTLIST:
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case PrimitiveTopology::PT_LINELIST:
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case PrimitiveTopology::PT_TRIANGLELIST:
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		default:
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		}

		// 设置Shader
		if (mCurrentShader)
		{
			mCurrentShader->ApplyShader();
		}

		DX11RenderOperationState ros;

		//光栅化阶段
		if (mRasterizer)
		{
			deviceContext->RSSetState(mRasterizer.Get());
		}

		//像素着色阶段
		/****设置纹理****/
		if (mTextureSettingChanged)
		{
			mTextureSettingChanged = false;
			ros.mTexturesCount = mTextureNum < MAX_TEXTURES ? mTextureNum : MAX_TEXTURES;
			for (size_t i = 0; i < ros.mTexturesCount; i++)
			{
				D3D11Texture* texture = dynamic_cast<D3D11Texture*>(mTextures[i]);
				if (texture)
				{
					ros.mTextures[i] = texture->GetTextureSRV();
				}
			}
			if (ros.mTexturesCount > 0)
			{
				deviceContext->PSSetShaderResources(0u, (UINT)ros.mTexturesCount, &ros.mTextures[0]);
			}
		}
		/****纹理结束****/
		if (mSamplerStates)
		{
			deviceContext->PSSetSamplers(0, 1, mSamplerStates.GetAddressOf());
		}

		//输出混合阶段
		if (mBlendDescChanged)
		{
			mBlendDescChanged = false;
		}
		if (mDepthStencilDescChanged)
		{
			mDepthStencilDescChanged = false;
		}

		deviceContext->DrawIndexed(rp.mIndicesCount, 0u, 0);

		//关闭混合
		deviceContext->OMSetBlendState(0, 0, 0xffffffff);

		//恢复默认的渲染状态
		deviceContext->RSSetState(nullptr);
	}
	HRESULT D3D11RenderSystem::CreateD3D11Device() const
	{
		HRESULT hr = mD3D11Device->Initialize();
		return hr;
	}
}