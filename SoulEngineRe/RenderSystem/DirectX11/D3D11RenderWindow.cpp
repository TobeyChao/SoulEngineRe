#include "D3D11RenderWindow.h"
#include "../../Platform/Win/WindowsUtils.h"
namespace Soul
{
	D3D11RenderWindow::D3D11RenderWindow(D3D11Device& device)
		:
		mDx11Device(device),
		mMsaaQuality(0),
		mSampleCount(0)
	{
		ZeroMemory(&mSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		mVSync = false;

		mHWnd = 0;
		mWindowedWinStyle = 0;
		mFullscreenWinStyle = 0;
	}
	D3D11RenderWindow::~D3D11RenderWindow()
	{
		// C++对象被销毁之前，销毁窗口对象  
		if (mHWnd != NULL && ::IsWindow(mHWnd))
		{
			// Tell system to destroy hWnd and Send WM_DESTROY to wndproc
			DestroyWindow(mHWnd);
		}
		mBackBuffer.Reset();
		mRenderTargetView.Reset();
		mDepthStencilView.Reset();
		mSwapChain.Reset();

	}
	bool D3D11RenderWindow::Initialize(const json& createParams)
	{
		mWindowedWinStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		mFullscreenWinStyle = WS_POPUP;
		// get handle to exe file
		HINSTANCE hInstance = GetModuleHandle(NULL);
		std::wstring wndClassName(L"SoulWndClass");
		std::wstring appName(L"SoulWin");
		std::wstring icon;

		if (createParams.find("fullscreen") != createParams.end())
		{
			mIsFullScreen = createParams["fullscreen"];
		}
		if (createParams.find("window_title") != createParams.end())
		{
			mName = createParams["window_title"];
			appName = StringToWstring(mName);
		}
		if (createParams.find("icon") != createParams.end())
		{
			std::string iconName = createParams["icon"];
			icon = StringToWstring(iconName);
		}
		if (createParams.find("screen_width") != createParams.end())
		{
			mWidth = createParams["screen_width"];
		}
		if (createParams.find("screen_height") != createParams.end())
		{
			mHeight = createParams["screen_height"];
		}
		if (createParams.find("vsync") != createParams.end())
		{
			mVSync = createParams["vsync"];
		}

		// register window class
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Platform::WindowsUtils::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);;
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = wndClassName.c_str();
		wcex.hIconSm = 0;

		// if there is an icon, load it
		wcex.hIcon = (HICON)LoadImage(hInstance, icon.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

		RegisterClassExW(&wcex);

		// calculate client size
		RECT clientSize;
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = mWidth;
		clientSize.bottom = mHeight;

		AdjustWindowRect(&clientSize, GetWndStyle(mIsFullScreen), FALSE);

		const int realWidth = clientSize.right - clientSize.left;
		const int realHeight = clientSize.bottom - clientSize.top;

		mLeft = mIsFullScreen ? 0 : (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
		mTop = mIsFullScreen ? 0 : (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

		// create window & get hWnd
		mHWnd = CreateWindowW(wndClassName.c_str(), appName.c_str(), GetWndStyle(mIsFullScreen),
			mLeft, mTop, realWidth, realHeight,
			nullptr, nullptr, hInstance, this);

		if (!mHWnd)
		{
			MessageBoxW(nullptr, L"Create Window Failed", L"Error", MB_OK | MB_ICONWARNING);
			return false;
		}

		ShowWindow(mHWnd, SW_SHOWNORMAL);
		UpdateWindow(mHWnd);

		// fix ugly ATI driver bugs. Thanks to ariaci
		MoveWindow(mHWnd, mLeft, mTop, realWidth, realHeight, TRUE);

		if (FAILED(CreateSwapChain(createParams)))
		{
			MessageBoxW(nullptr, L"Create SwapChain Failed", L"Error", MB_OK | MB_ICONWARNING);
			return false;
		}
		if (FAILED(CreateDx11RenderResource()))
		{
			MessageBoxW(nullptr, L"Create D3D11 Render Resource Failed", L"Error", MB_OK | MB_ICONWARNING);
			return false;
		}

		Platform::WindowsUtils::HWnd = mHWnd;

		return true;
	}
	void D3D11RenderWindow::UnInitialize()
	{
	}

	void D3D11RenderWindow::SwapBuffers()
	{
		mSwapChain->Present(mVSync ? 1 : 0, 0);
	}

	void D3D11RenderWindow::Update()
	{
		RenderWindow::Update();
	}

	HRESULT D3D11RenderWindow::CreateSwapChain(const json& createParams)
	{
		// Use the factory to create an adapter for the primary graphics interface (video card).
		IDXGIAdapter* adapter = mDx11Device.GetDXGIAdapter();

		// Enumerate the primary adapter output (monitor).
		IDXGIOutput* adapterOutput;
		HRESULT hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr))
		{
			return hr;
		}
		unsigned int numModes = 0;
		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(hr))
		{
			return hr;
		}
		// Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];

		// Now fill the display mode list structures.
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(hr))
		{
			return hr;
		}
		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		unsigned int numerator = 0;
		unsigned int denominator = 1;
		for (UINT i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == mWidth)
			{
				if (displayModeList[i].Height == mHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
					break;
				}
			}
		}
		delete[] displayModeList;

		DXGI_MODE_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width = mWidth;
		bufferDesc.Height = mHeight;
		bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		// Set the refresh rate of the back buffer.
		if (mVSync)
		{
			bufferDesc.RefreshRate.Numerator = numerator;
			bufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			bufferDesc.RefreshRate.Numerator = 0;
			bufferDesc.RefreshRate.Denominator = 1;
		}
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// 检测 MSAA支持的质量等级
		mSampleCount = 0;
		if (createParams.find("msaa") != createParams.end())
		{
			mSampleCount = createParams["msaa"];
			if (mSampleCount > 0)
			{
				mDx11Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mSampleCount, &mMsaaQuality);
			}
		}

		ZeroMemory(&mSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		mSwapChainDesc.BufferCount = 1;
		mSwapChainDesc.BufferDesc = bufferDesc;
		mSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		mSwapChainDesc.OutputWindow = mHWnd;
		// 当多重采样抗锯齿sampleCount大于0时并且支持才可以开启多重采样抗锯齿
		if (mSampleCount > 0 && mMsaaQuality > 0)
		{
			mSwapChainDesc.SampleDesc.Count = mSampleCount;
			mSwapChainDesc.SampleDesc.Quality = mMsaaQuality - 1;
		}
		else
		{
			mSwapChainDesc.SampleDesc.Count = 1;
			mSwapChainDesc.SampleDesc.Quality = 0;
		}

		mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		mSwapChainDesc.Windowed = !mIsFullScreen;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		hr = mDx11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}

		hr = mDx11Device.GetDXGIFactory()->CreateSwapChain(dxgiDevice.Get(), &mSwapChainDesc, &mSwapChain);
		return hr;
	}

	HRESULT D3D11RenderWindow::CreateDx11RenderResource()
	{
		mBackBuffer.Reset();
		HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mBackBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(hr))
		{
			return hr;
		}
		hr = mDx11Device->CreateRenderTargetView(mBackBuffer.Get(), nullptr, &mRenderTargetView);
		if (FAILED(hr))
		{
			return hr;
		}

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		depthBufferDesc.Width = mWidth;
		depthBufferDesc.Height = mHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.MiscFlags = 0;
		if (mSampleCount > 0 && mMsaaQuality > 0)
		{
			depthBufferDesc.SampleDesc.Count = mSampleCount;
			depthBufferDesc.SampleDesc.Quality = mMsaaQuality - 1;
		}
		else
		{
			depthBufferDesc.SampleDesc.Count = 1;
			depthBufferDesc.SampleDesc.Quality = 0;
		}

		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		hr = mDx11Device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
		if (FAILED(hr))
		{
			return hr;
		}

		hr = mDx11Device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, mDepthStencilView.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}

		return hr;
	}
}