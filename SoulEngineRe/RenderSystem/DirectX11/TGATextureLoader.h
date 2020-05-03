#pragma once

#include <d3d11_1.h>
#include <stdint.h>
#include <stdio.h>

namespace Soul
{
	HRESULT CreateTGATextureFromFileEX(
		ID3D11Device* d3dDevice,
		ID3D11DeviceContext * deviceContext,
		const wchar_t* szFileName,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView,
		size_t maxsize = 0);
}