#pragma once
#include "../../SoulMain/Resource/TextureManager.h"
#include "D3D11Device.h"

namespace Soul
{
	class D3D11TextureManager : public TextureManager
	{
	public:
		D3D11TextureManager(D3D11Device* device);
		ITexture* LoadTexture(std::wstring imageName) override;
	private:
		D3D11Device* mDevice;
	};
}
