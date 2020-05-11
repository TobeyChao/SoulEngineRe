#include "D3D11RenderSystem.h"
#include "D3D11GPUBuffer.h"
#include "D3D11ShaderManager.h"
#include "D3D11Texture.h"
#include "D3D11GPUBufferManager.h"
#include "D3D11Shader.h"
#include "D3D11RenderWindow.h"
#include "D3D11TextureManager.h"
#include "D2DRenderSystem.h"
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
		mTextureSettingChanged(false),
		mStencilRef(0)
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

		// 创建D3DDevice
		if (FAILED(CreateD3D11Device()))
		{
			// 创建Device失败
		}

		// 默认混合
		ZeroMemory(&mBlendDesc, sizeof(mBlendDesc));
		mBlendDesc.AlphaToCoverageEnable = true;
		mBlendDesc.IndependentBlendEnable = false;
		mBlendDesc.RenderTarget[0].BlendEnable = false;
		mBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		if (FAILED((*mD3D11Device)->CreateBlendState(&mBlendDesc, mDefaultBlendState.ReleaseAndGetAddressOf())))
		{
		}

		// 默认深度模板
		ZeroMemory(&mDepthStencilDesc, sizeof(mDepthStencilDesc));
		mDepthStencilDesc.DepthEnable = TRUE;
		mDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		mDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		mDepthStencilDesc.StencilEnable = FALSE;
		mDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		mDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		mDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		mDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		mDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		mDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		mDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		mDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		mDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		mDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		if (FAILED((*mD3D11Device)->CreateDepthStencilState(&mDepthStencilDesc, mDefaultDepthStencilState.ReleaseAndGetAddressOf())))
		{
		}

		// 默认光栅化器
		ZeroMemory(&mRasterizerDesc, sizeof(mRasterizerDesc));
		mRasterizerDesc.FrontCounterClockwise = false;
		mRasterizerDesc.DepthClipEnable = true;
		mRasterizerDesc.MultisampleEnable = true;
		mRasterizerDesc.CullMode = D3D11_CULL_BACK;
		mRasterizerDesc.FillMode = D3D11_FILL_SOLID;
		if (FAILED((*mD3D11Device)->CreateRasterizerState(&mRasterizerDesc, mDefaultRasterizer.ReleaseAndGetAddressOf())))
		{
			// 创建RasterizerState失败
		}

		// 默认采样器
		D3D11_SAMPLER_DESC dsd = {};
		dsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		dsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		dsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		dsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		dsd.MaxAnisotropy = ((*mD3D11Device)->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2u;
		dsd.MaxLOD = FLT_MAX;
		dsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		if (FAILED((*mD3D11Device)->CreateSamplerState(&dsd, mDefaultSamplerStates.ReleaseAndGetAddressOf())))
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
		mShaderManager->AddShader(L"SkyBox");
		mShaderManager->AddShader(L"Particle");
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
		bool ret = window->Initialize(config);
		if (!ret)
		{
			return nullptr;
		}
		// 窗体创建完才可以初始化渲染系统
		mRenderSystem2D = new D2DRenderSystem(*window);
		mRenderSystem2D->Initialize();
		// 添加窗口为渲染目标
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
	void D3D11RenderSystem::SetBlendType(const BlendType& bt)
	{
		switch (bt)
		{
		case BlendType::BT_DEFAULT:
			mBlendDescChanged = false;
			break;
		case BlendType::BT_TRANSPARENT:
			ZeroMemory(&mBlendDesc, sizeof(mBlendDesc));
			// Color = SrcAlpha * SrcColor + (1 - SrcAlpha) * DestColor 
			// Alpha = SrcAlpha
			mBlendDesc.AlphaToCoverageEnable = false;
			mBlendDesc.RenderTarget[0].BlendEnable = true;
			mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			mBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			mBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			mBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			mBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			mBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			mBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			mBlendDescChanged = true;
			break;
		case BlendType::BT_ADD:
			ZeroMemory(&mBlendDesc, sizeof(mBlendDesc));
			// 加法混合模式
			// Color = SrcColor + DestColor
			// Alpha = SrcAlpha
			mBlendDesc.AlphaToCoverageEnable = false;
			mBlendDesc.RenderTarget[0].BlendEnable = true;
			mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			mBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			mBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			mBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			mBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			mBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			mBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			mBlendDescChanged = true;
			break;
		default:
			mBlendDescChanged = false;
			break;
		}
	}
	void D3D11RenderSystem::SetRasterizerType(const RasterizerType& rt)
	{
		switch (rt)
		{
		case RasterizerType::RT_CULL_CLOCKWISE:
			ZeroMemory(&mRasterizerDesc, sizeof(mRasterizerDesc));
			mRasterizerDesc.FrontCounterClockwise = false;
			mRasterizerDesc.DepthClipEnable = true;
			mRasterizerDesc.MultisampleEnable = true;
			mRasterizerDesc.CullMode = D3D11_CULL_FRONT;
			mRasterizerDesc.FillMode = D3D11_FILL_SOLID;
			mRasterizerDescChanged = true;
			break;
		case RasterizerType::RT_CULL_COUNTERCLOCKWISE:
			ZeroMemory(&mRasterizerDesc, sizeof(mRasterizerDesc));
			mRasterizerDesc.FrontCounterClockwise = false;
			mRasterizerDesc.DepthClipEnable = true;
			mRasterizerDesc.MultisampleEnable = true;
			mRasterizerDesc.CullMode = D3D11_CULL_BACK;
			mRasterizerDesc.FillMode = D3D11_FILL_SOLID;
			mRasterizerDescChanged = true;
			break;
		case RasterizerType::RT_CULL_NONE:
			ZeroMemory(&mRasterizerDesc, sizeof(mRasterizerDesc));
			mRasterizerDesc.FrontCounterClockwise = false;
			mRasterizerDesc.DepthClipEnable = true;
			mRasterizerDesc.MultisampleEnable = true;
			mRasterizerDesc.CullMode = D3D11_CULL_NONE;
			mRasterizerDesc.FillMode = D3D11_FILL_SOLID;
			mRasterizerDescChanged = true;
			break;
		case RasterizerType::RT_WIREFRAME:
			ZeroMemory(&mRasterizerDesc, sizeof(mRasterizerDesc));
			mRasterizerDesc.FrontCounterClockwise = false;
			mRasterizerDesc.DepthClipEnable = true;
			mRasterizerDesc.MultisampleEnable = true;
			mRasterizerDesc.CullMode = D3D11_CULL_NONE;
			mRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			mRasterizerDescChanged = true;
			break;
		case RasterizerType::RT_DEFAULT:
			mRasterizerDescChanged = false;
			break;
		default:
			mRasterizerDescChanged = false;
			break;
		}
	}
	void D3D11RenderSystem::SetDepthStencilType(const DepthStencilType& dst)
	{
		switch (dst)
		{
		case DepthStencilType::DST_DEFAULT:
			mDepthStencilDescChanged = false;
			break;
		case DepthStencilType::DST_LESS_EQUAL:
			// 允许使用深度值一致的像素进行替换的深度/模板状态
			// 该状态用于绘制天空盒，因为深度值为1.0时可以通过深度测试
			mDepthStencilDesc.DepthEnable = true;
			mDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			mDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			mDepthStencilDesc.StencilEnable = false;
			mDepthStencilDescChanged = true;
			break;
		case DepthStencilType::DST_NO_DEPTH_WRITE:
			// 进行深度测试，但不写入深度值的状态
			// 若绘制非透明物体时，应使用默认状态
			// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
			// 并且确保较前的非透明物体可以阻挡较后的一切物体
			mDepthStencilDesc.DepthEnable = true;
			mDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			mDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

			mDepthStencilDesc.StencilEnable = false;
			mDepthStencilDescChanged = true;
			break;
		case DepthStencilType::DST_NO_DOUBLE_BLEND:
			// 无二次混合深度/模板状态
			// 允许默认深度测试
			// 通过自递增使得原来StencilRef的值只能使用一次，实现仅一次混合
			mDepthStencilDesc.DepthEnable = true;
			mDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			mDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

			mDepthStencilDesc.StencilEnable = true;
			mDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			mDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			mDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			mDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			mDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
			mDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
			mDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			mDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			mDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
			mDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			mDepthStencilDescChanged = true;
			break;
		default:
			break;
		}
	}
	void D3D11RenderSystem::SetStencilRef(UINT stencilRef)
	{
		mStencilRef = stencilRef;
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
		HRESULT hr;

		//输出混合阶段
		if (mBlendDescChanged)
		{
			mBlendDescChanged = false;
			hr = (*mD3D11Device)->CreateBlendState(&mBlendDesc, ros.mBlendState.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
			}
		}

		//光栅化阶段
		if (mRasterizerDescChanged)
		{
			mRasterizerDescChanged = false;
			hr = (*mD3D11Device)->CreateRasterizerState(&mRasterizerDesc, ros.mRasterizer.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				// 创建RasterizerState失败
			}
		}

		if (mDepthStencilDescChanged)
		{
			mDepthStencilDescChanged = false;
			hr = (*mD3D11Device)->CreateDepthStencilState(&mDepthStencilDesc, ros.mDepthStencilState.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
			}
		}

		//像素着色阶段
		// 纹理
		if (!mTextureSettingChanged && mTextureNum == 0)
		{
			deviceContext->PSSetShaderResources(0u, 0u, nullptr);
			deviceContext->PSSetSamplers(0, 0, nullptr);
		}
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
				// 采样
				if (mDefaultSamplerStates)
				{
					deviceContext->PSSetSamplers(0, 1, mDefaultSamplerStates.GetAddressOf());
				}
			}
			mTextureNum = 0;
			for (size_t i = 0; i < MAX_TEXTURES; i++)
			{
				mTextures[i] = nullptr;
			}
		}

		// 混合
		if (ros.mBlendState)
		{
			deviceContext->OMSetBlendState(ros.mBlendState.Get(), 0, 0xffffffff);
		}
		else
		{
			deviceContext->OMSetBlendState(nullptr, 0, 0xffffffff);
		}

		// 光栅化
		if (ros.mRasterizer)
		{
			deviceContext->RSSetState(ros.mRasterizer.Get());
		}
		else
		{
			deviceContext->RSSetState(mDefaultRasterizer.Get());
		}

		// 深度模板
		if (ros.mDepthStencilState)
		{
			deviceContext->OMSetDepthStencilState(ros.mDepthStencilState.Get(), mStencilRef);
			mStencilRef = 0;
		}
		else
		{
			deviceContext->OMSetDepthStencilState(nullptr, 0);
		}

		deviceContext->DrawIndexed(rp.mIndicesCount, 0u, 0);

		// 恢复默认的渲染状态
		//deviceContext->OMSetBlendState(0, 0, 0xffffffff);
		//deviceContext->RSSetState(nullptr);
		//deviceContext->OMSetDepthStencilState(0, 0);
		//deviceContext->PSSetSamplers(static_cast<UINT>(0), static_cast<UINT>(0), 0);
	}
	HRESULT D3D11RenderSystem::CreateD3D11Device() const
	{
		HRESULT hr = mD3D11Device->Initialize();
		return hr;
	}
}