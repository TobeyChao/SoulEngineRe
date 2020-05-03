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
		ITexture* pTexture = new D3D11Texture(mDevice, imageName);
		mTextureList[imageName] = pTexture;
		return pTexture;
	}
}
