#pragma once
#include "RenderWindow.h"
#include "RenderParameter.h"
#include "../Core/SColorf.h"
#include "../Resource/ResourceGroupManager.h"
#include "../Resource/TextureManager.h"
#include "GPUBufferManager.h"
#include "ShaderManager.h"

namespace Soul
{
	class RenderSystem
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();
		// 初始化操作，主要完成配置赋值
		virtual void Initialize(const std::string& initConfig);
		[[nodiscard]] virtual const std::string& GetRenderSystemName() const = 0;
		virtual RenderWindow* CreateRenderWindow(const json& config) = 0;
		// 获取渲染窗口配置
		json& GetRenderWindowDescRef();
		// 设置RenderTarget
		virtual void SetRenderTarget(RenderTarget* renderTarget);
		void AddRenderTarget(RenderTarget* renderTarget);
		void RemoveRenderTarget(const std::string& name);
		RenderTarget* GetRenderTargetByName(const std::string& name) const;
		// 设置Viewport
		virtual void SetViewport(Viewport* viewport) = 0;
		// 更新Render Target，由SAdmin调用
		void UpdateRenderTarget();
		// 交换Buffer
		void SwapRenderTargetBuffer();
		// 设置纹理
		virtual void SetTexture(size_t slot, ITexture* texture) = 0;
		// 设置混合
		virtual void SetBlendType(const BlendType& bt) = 0;
		// 设置光栅化
		virtual void SetRasterizerType(const RasterizerType& rt) = 0;
		// 设置深度模板
		virtual void SetDepthStencilType(const DepthStencilType& dst) = 0;

		virtual void SetStencilRef(UINT stencilRef) = 0;

		// 设置着色器
		virtual void BindShader(Shader* shader) = 0;

		virtual void Render(const RenderParameter& rp);
	protected:
		// 配置
		json mConfig;
		// 所有的RenderTargert
		std::map<std::string, RenderTarget*> mRenderTargets;
		// 当前激活的RenderTarget
		RenderTarget* mActiveRenderTarget;
		// 视口
		Viewport* mActiveViewport;
		// 着色器管理器
		ShaderManager* mShaderManager;
		// 纹理管理器
		TextureManager* mTextureManager;
		// GPUBuffer管理器
		GPUBufferManager* mGpuBufferManager;
	};
}