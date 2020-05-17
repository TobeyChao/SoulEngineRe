#include "D3D11TextureManager.h"
#include "D3D11Texture.h"

namespace Soul
{
	D3D11TextureManager::D3D11TextureManager(D3D11Device* device)
		:
		mDevice(device)
	{
	}
	ITexture* D3D11TextureManager::LoadTexture(std::wstring imageName)
	{
		ITexture* pTexture = new D3D11Texture(*mDevice, imageName);
		mTextureList[imageName] = pTexture;
		return pTexture;
	}
	ITexture* D3D11TextureManager::CreateTexture(const std::wstring& textureName, const Core::SDimension2& imageSize, bool generateMips)
	{
		ITexture* pTexture = new D3D11RenderTexture(*mDevice, textureName, imageSize, generateMips);
		mTextureList[textureName] = pTexture;
		return pTexture;
	}
}