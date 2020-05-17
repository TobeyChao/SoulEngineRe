#pragma once
#include "D3D11PCH.h"
#include "D3D11Device.h"
#include "D3D11RenderTarget.h"
#include "../../SoulMain/RenderSystem/RenderWindow.h"
namespace Soul
{
	class D3D11RenderWindow final : public RenderWindow, public D3D11RenderTarget
	{
	public:
		D3D11RenderWindow(D3D11Device& device);
		~D3D11RenderWindow();
		bool Initialize(const json& createParams) override;
		void UnInitialize() override;
		void SwapBuffers() override;
		void Update() override;
		void Clear() override;
		IDXGISwapChain* GetSwapChain() const
		{
			return mSwapChain.Get();
		}
		DWORD GetWndStyle(bool fullscreen) const
		{
			return fullscreen ? mFullscreenWinStyle : mWindowedWinStyle;
		}
		ID3D11RenderTargetView* GetRenderTargetView() const override
		{
			return mRenderTargetView.Get();
		}
		ID3D11DepthStencilView* GetDepthStencilView() const override
		{
			return mDepthStencilView.Get();
		}
		HWND GetHWnd() const
		{
			return mHWnd;
		}
	private:
		HRESULT CreateSwapChain(const json& createParams);
		HRESULT CreateD3D11RenderResource();

	private:
		D3D11Device& mDx11Device;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;

		// 交换链
		Microsoft::WRL::ComPtr<IDXGISwapChain>	mSwapChain;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain1;// D3D11.1交换链
		UINT mSampleCount;
		UINT mMsaaQuality;

		// 垂直同步
		bool mVSync;

		HWND	mHWnd;
		DWORD	mWindowedWinStyle;
		DWORD	mFullscreenWinStyle;
	};
}