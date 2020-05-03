#pragma once
#include "ResourceManager.h"
#include <map>

#include "../RenderSystem/ITexture.h"
#include "../Utils/Singleton.h"

namespace Soul
{
	//Driver Type
	enum E_TEXTURE_TYPES
	{
		TEXTURE_DIRECTX11
	};

	class TextureManager : public ResourceManager, public Singleton<TextureManager>
	{
	public:
		TextureManager();
		virtual ~TextureManager();
		virtual ITexture* LoadTexture(std::wstring imageName) = 0;
		ITexture* GetTexture(std::wstring imageName);
	protected:
		std::map<std::wstring, ITexture*> mTextureList;
	};
}
