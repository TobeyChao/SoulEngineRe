#include "D2DRenderSystem.h"

namespace Soul
{
	D2DRenderSystem::D2DRenderSystem(const D3D11RenderWindow& D3D11RenderWindow)
		:
		mD3D11RenderWindow(D3D11RenderWindow)
	{
	}

	D2DRenderSystem::~D2DRenderSystem()
	{
		mFactory.Reset();
		mRenderTarget.Reset();
		mBrush.Reset();

		mWriteFactory.Reset();
		mTextFormat.Reset();
	}

	bool D2DRenderSystem::Initialize()
	{
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mFactory.GetAddressOf())))
		{
			return false;
		}

		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(mWriteFactory.GetAddressOf()))))
		{
			return false;
		}

		mRenderTarget.Reset();

		// 为D2D创建DXGI表面渲染目标
		Microsoft::WRL::ComPtr<IDXGISurface1> surface;
		mD3D11RenderWindow.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface1), reinterpret_cast<void**>(surface.ReleaseAndGetAddressOf()));

		D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

		ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

		renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
		renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

		HRESULT hr = mFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &renderTargetProperties, mRenderTarget.GetAddressOf());
		surface.Reset();

		if (hr == E_NOINTERFACE)
		{
			MessageBox(NULL, L"DirectX11.1 Not Support!", L"Error", MB_OK | MB_ICONWARNING);
			return false;
		}
		else if (hr == S_OK)
		{
			if (FAILED(mRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), mBrush.GetAddressOf())))
			{
				MessageBox(NULL, L"Create Solid Color Brush Failed!", L"Error", MB_OK | MB_ICONWARNING);
				return false;
			}
			if (FAILED(mWriteFactory->CreateTextFormat(L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn",
				mTextFormat.GetAddressOf())))
			{
				MessageBox(NULL, L"Create Text Format!", L"Error", MB_OK | MB_ICONWARNING);
				return false;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	void D2DRenderSystem::DrawTextW(const std::wstring& text, const Core::SVector2& pos)
	{
		if (mRenderTarget)
		{
			mRenderTarget->BeginDraw();
			D2D1_SIZE_F size = mRenderTarget->GetSize();
			mRenderTarget->DrawTextW(text.c_str(), (UINT32)text.size(), mTextFormat.Get(),
				D2D1_RECT_F{ pos.x, pos.y, size.width, size.height }, mBrush.Get());
			mRenderTarget->EndDraw();
		}
	}
}
