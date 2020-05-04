#pragma once
#include "D3D11PCH.h"
namespace Soul
{
	class InputLayouts
	{
	public:
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPos[1];
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosCol[2];
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosTex[2];
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosTexNor[3];
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosTexCol[3];
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosNorCol[3];
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosTexNorCol[4];
	};
}