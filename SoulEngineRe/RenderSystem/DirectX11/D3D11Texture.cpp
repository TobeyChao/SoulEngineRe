#include "D3D11Texture.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "TGATextureLoader.h"
#include <iostream>

namespace Soul
{
	D3D11Texture::D3D11Texture(D3D11Device& device, const std::wstring& filePath)
		:
		ITexture(filePath),
		mTextureSRV(nullptr),
		mD3DDevice(device)
	{
		HRESULT hr;
		std::wstring ext = filePath.substr(filePath.find_last_of(L".") + 1);
		//Load Texture
		if (ext == L"dds")
		{
			hr = DirectX::CreateDDSTextureFromFile(mD3DDevice.operator->(),
				filePath.c_str(), mTex.GetAddressOf(), mTextureSRV.GetAddressOf());
		}
		else if (ext == L"tga")
		{
			hr = Soul::CreateTGATextureFromFileEX(mD3DDevice.operator->(), mD3DDevice.GetDeviceContext(),
				filePath.c_str(), mTex.GetAddressOf(), mTextureSRV.GetAddressOf());
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(mD3DDevice.operator->(),
				filePath.c_str(), mTex.GetAddressOf(), mTextureSRV.GetAddressOf());
		}
		if (SUCCEEDED(hr))
			std::wcout << L"LOAD " << filePath.c_str() << L"...LOAD SUCCEEDED" << std::endl;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		mTex.As(&texture);
		CD3D11_TEXTURE2D_DESC catDesc;
		texture->GetDesc(&catDesc);
		//Set Texture Size
		mTextureSize.width = catDesc.Width;
		mTextureSize.height = catDesc.Height;
	}

	D3D11Texture::~D3D11Texture()
	{
		mTextureSRV.Reset();
		mTex.Reset();
	}
	ID3D11ShaderResourceView* D3D11Texture::GetTextureSRV() const
	{
		return mTextureSRV.Get();
	}

	D3D11RenderTexture::D3D11RenderTexture(D3D11Device& device, const std::wstring& textureName, const Core::SDimension2& imageSize, bool generateMips)
		:
		ITexture(textureName),
		mD3DDevice(device),
		mGenerateMips(generateMips)
	{
		mWidth = imageSize.width;
		mHeight = imageSize.height;
		mName = WstringToString(textureName);
		mOutputTextureSRV.Reset();
		mOutputTextureRTV.Reset();
		mOutputTextureDSV.Reset();

		HRESULT hr;
		// ******************
		// 1. 创建纹理
		//

		mTextureSize = imageSize;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		D3D11_TEXTURE2D_DESC texDesc;

		texDesc.Width = mTextureSize.width;
		texDesc.Height = mTextureSize.height;
		texDesc.MipLevels = (mGenerateMips ? 0 : 1);	// 0为完整mipmap链
		texDesc.ArraySize = 1;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// 现在texture用于新建纹理
		hr = device->CreateTexture2D(&texDesc, nullptr, texture.GetAddressOf());
		if (FAILED(hr))
			return;
		// ******************
		// 2. 创建纹理对应的渲染目标视图
		//

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateRenderTargetView(texture.Get(), &rtvDesc, mOutputTextureRTV.GetAddressOf());
		if (FAILED(hr))
			return;

		// ******************
		// 3. 创建纹理对应的着色器资源视图
		//

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;	// 使用所有的mip等级

		hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, mOutputTextureSRV.GetAddressOf());
		if (FAILED(hr))
			return;

		// ******************
		// 4. 创建与纹理等宽高的深度/模板缓冲区和对应的视图
		//

		texDesc.Width = mTextureSize.width;
		texDesc.Height = mTextureSize.height;
		texDesc.MipLevels = 0;
		texDesc.ArraySize = 1;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTex;
		hr = device->CreateTexture2D(&texDesc, nullptr, depthTex.GetAddressOf());
		if (FAILED(hr))
			return;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Format = texDesc.Format;
		dsvDesc.Flags = 0;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(depthTex.Get(), &dsvDesc, mOutputTextureDSV.GetAddressOf());
		if (FAILED(hr))
			return;
	}

	D3D11RenderTexture::~D3D11RenderTexture()
	{
	}
}