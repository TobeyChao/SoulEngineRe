#include "TextureManager.h"
#include "ResourceGroupManager.h"

namespace Soul
{
	template<> TextureManager* Singleton<TextureManager>::mSingleton = nullptr;
	TextureManager::TextureManager()
	{
		ResourceGroupManager::GetInstance().RegisterResourceManager(L"Texture", this);
	}
	TextureManager::~TextureManager()
	{
		for (auto it : mTextureList)
		{
			ITexture* texture = it.second;
			if (texture)
			{
				delete texture;
				texture = nullptr;
			}
		}
		mTextureList.clear();
		ResourceGroupManager::GetInstance().UnregisterResourceManager(L"Texture");
	}
	ITexture* TextureManager::GetTexture(std::wstring imageName)
	{
		auto it = mTextureList.find(imageName);
		if (it != mTextureList.end())
		{
			return it->second;
		}
		return LoadTexture(imageName);
	}
}