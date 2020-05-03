#pragma once
#include "D3D11PCH.h"
#include "D3D11Device.h"
#include "../../SoulMain/RenderSystem/RenderWindow.h"
namespace Soul
{
	class D3D11RenderWindow final : public RenderWindow
	{
	public:
		D3D11RenderWindow(D3D11Device& device);
		~D3D11RenderWindow();
		bool Initialize(const json& createParams) override;
		void UnInitialize() override;
		void SwapBuffers() override;
		void Update() override;
		IDXGISwapChain* GetSwapChain() const
		{
			return mSwapChain.Get();
		}
		DWORD GetWndStyle(bool fullscreen) const
		{
			return fullscreen ? mFullscreenWinStyle : mWindowedWinStyle;
		}
		ID3D11RenderTargetView* GetRenderTargetView() const
		{
			return mRenderTargetView.Get();
		}
		ID3D11DepthStencilView* GetDepthStencilView() const
		{
			return mDepthStencilView.Get();
		}
		HWND GetHWnd() const
		{
			return mHWnd;
		}
	private:
		HRESULT CreateSwapChain(const json& createParams);
		HRESULT CreateDx11RenderResource();

	private:
		D3D11Device& mDx11Device;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;

		// 交换链
		Microsoft::WRL::ComPtr<IDXGISwapChain>	mSwapChain;
		UINT mSampleCount;
		UINT mMsaaQuality;
		// 描述交换链
		DXGI_SWAP_CHAIN_DESC mSwapChainDesc;
		// 垂直同步
		bool mVSync;

		HWND	mHWnd;
		DWORD	mWindowedWinStyle;
		DWORD	mFullscreenWinStyle;
	};
}