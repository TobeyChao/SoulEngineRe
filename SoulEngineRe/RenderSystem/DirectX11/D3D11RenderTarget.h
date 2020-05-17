#pragma once
#include "D3D11PCH.h"

namespace Soul
{
	class D3D11RenderTarget
	{
	public:
		virtual ID3D11RenderTargetView* GetRenderTargetView() const = 0;
		virtual ID3D11DepthStencilView* GetDepthStencilView() const = 0;
	};
}