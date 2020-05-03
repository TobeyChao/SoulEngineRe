#pragma once
#include "D3D11RenderSystem.h"

namespace Soul
{
	class D3D11Texture final : public ITexture
	{
	public:
		D3D11Texture(D3D11Device* device, const std::wstring& filePath);
		~D3D11Texture();
		[[nodiscard]] ID3D11ShaderResourceView* GetTextureSRV() const;
	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> mTex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureSRV;
		D3D11Device* mD3DDevice;
	};
}
