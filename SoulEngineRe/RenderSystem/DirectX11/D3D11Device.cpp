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
		mDevice1.Reset();
		mDeviceContext1.Reset();
		mDXGIFactory2.Reset();
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

		mDXGIFactory.As(&mDXGIFactory2);
		
		UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;;
#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
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

		// 驱动类型数组
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL maxRequestedFeatureLevel;

		D3D_DRIVER_TYPE d3dDriverType;
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			d3dDriverType = driverTypes[driverTypeIndex];
			result = D3D11CreateDevice(
				nullptr,
				d3dDriverType,
				nullptr,
				flags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				mDevice.GetAddressOf(),
				&maxRequestedFeatureLevel,
				mDeviceContext.ReleaseAndGetAddressOf());

			if (result == E_INVALIDARG)
			{
				// Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
				result = D3D11CreateDevice(
					nullptr,
					d3dDriverType,
					nullptr,
					flags,
					&featureLevels[1],
					ARRAYSIZE(featureLevels) - 1,
					D3D11_SDK_VERSION,
					mDevice.GetAddressOf(),
					&maxRequestedFeatureLevel,
					mDeviceContext.GetAddressOf());
			}

			if (SUCCEEDED(result))
				break;
		}
		

		// 如果包含，则说明支持D3D11.1
		if (mDXGIFactory2 != nullptr)
		{
			mDevice.As(&mDevice1);
			mDeviceContext.As(&mDeviceContext1);
		}
		return result;
	}
}