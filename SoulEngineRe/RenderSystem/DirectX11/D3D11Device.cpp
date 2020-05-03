#include "D3D11Device.h"

namespace Soul
{
	D3D11Device::D3D11Device()
	{
	}
	D3D11Device::~D3D11Device()
	{
		mDevice.Reset();
		mDeviceContext.Reset();
		mDXGIFactory.Reset();
	}
	HRESULT D3D11Device::Initialize()
	{
		auto result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(mDXGIFactory.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			return result;
		}
		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = mDXGIFactory->EnumAdapters1(0, mDXGIAdapter.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return result;
		}
		UINT flags = 0;
#ifdef _DEBUG
		//flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			// determine feature levels
		D3D_FEATURE_LEVEL featureLevels[] =
		{
#if defined(_WIN32_WINNT_WIN8) && _WIN32_WINNT >= _WIN32_WINNT_WIN8
			D3D_FEATURE_LEVEL_11_1,
#endif
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		D3D_FEATURE_LEVEL maxRequestedFeatureLevel = D3D_FEATURE_LEVEL_9_1;

		result = D3D11CreateDevice(
			mDXGIAdapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			flags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			mDevice.ReleaseAndGetAddressOf(),
			&maxRequestedFeatureLevel,
			mDeviceContext.ReleaseAndGetAddressOf());

		return result;
	}
}