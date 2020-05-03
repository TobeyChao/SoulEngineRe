#pragma once
#include "../../SoulMain/RenderSystem/RenderSystem.h"
#include "D3D11Device.h"

namespace Soul
{
constexpr auto MAX_TEXTURES = 8;
	class D3D11RenderSystem : public RenderSystem
	{
	public:
		D3D11RenderSystem();
		~D3D11RenderSystem();
		void Initialize(const std::string& initConfig) override;
		[[nodiscard]] const std::string& GetRenderSystemName() const override;
		RenderWindow* CreateRenderWindow(const json& config) override;
		void SetRenderTarget(RenderTarget* renderTarget) override;
		void SetViewport(Viewport* viewport) override;
		void SetTexture(size_t slot, ITexture* texture) override;
		void SetBlendType(const BlendType& bt) override;
		void SetRasterizerType(const RasterizerType& rt) override;
		void SetDepthStencilType(const DepthStencilType& dst) override;
		void BindShader(Shader* shader) override;
		void Clear(const Core::SColorf& color) override;
		void Render(const RenderParameter& rp) override;
	private:
		HRESULT CreateD3D11Device() const;

	private:
		D3D11Device* mD3D11Device;

		D3D_DRIVER_TYPE mDriverType;
		D3D_FEATURE_LEVEL mFeatureLevel;

		D3D11_BLEND_DESC mBlendDesc;
		bool mBlendDescChanged;

		D3D11_RASTERIZER_DESC   mRasterizerDesc;
		bool mRasterizerDescChanged;

		UINT mStencilRef;
		D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc;
		bool mDepthStencilDescChanged;

		bool mTextureSettingChanged;

		Microsoft::WRL::ComPtr<ID3D11BlendState> mDefaultBlendState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mDefaultRasterizer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDefaultDepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mDefaultSamplerStates;

		// ²ÄÖÊ
		ITexture* mTextures[MAX_TEXTURES];
		size_t mTextureNum = 0;
		// ×ÅÉ«Æ÷
		Shader* mCurrentShader;
	};

	class DX11RenderOperationState
	{
	public:
		DX11RenderOperationState()
			:
			mSamplerStatesCount(0),
			mTexturesCount(0),
			mTextures()
		{}
		~DX11RenderOperationState() {}

		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerStates[MAX_TEXTURES];
		size_t mSamplerStatesCount;

		ID3D11ShaderResourceView* mTextures[MAX_TEXTURES];
		size_t mTexturesCount;
	};
}