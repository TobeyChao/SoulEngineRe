#pragma once
#include "../../SoulMain/RenderSystem/ShaderManager.h"
#include "D3D11Device.h"

namespace Soul
{
	class D3D11ShaderManager : public ShaderManager
	{
	public:
		D3D11ShaderManager(D3D11Device& device);
		~D3D11ShaderManager();
		void AddShader(const std::wstring& shaderName);
	private:
		D3D11Device& mDevice;
	};
}