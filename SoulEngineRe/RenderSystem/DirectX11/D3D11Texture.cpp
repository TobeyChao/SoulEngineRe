#include "D3D11Texture.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "TGATextureLoader.h"
#include <iostream>

namespace Soul
{
	D3D11Texture::D3D11Texture(D3D11Device* device, const std::wstring& filePath)
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
			hr = DirectX::CreateDDSTextureFromFile(mD3DDevice->operator->(),
				filePath.c_str(), mTex.GetAddressOf(), mTextureSRV.GetAddressOf());
		}
		else if (ext == L"tga")
		{
			hr = Soul::CreateTGATextureFromFileEX(mD3DDevice->operator->(), mD3DDevice->GetDeviceContext(),
				filePath.c_str(), mTex.GetAddressOf(), mTextureSRV.GetAddressOf());
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(mD3DDevice->operator->(),
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
}