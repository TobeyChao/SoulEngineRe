#include "D3D11ShaderManager.h"
#include "../../SoulMain/Resource/ResourceGroupManager.h"

#include "D3D11TextureShader.h"
#include "D3D11BasicShader.h"
#include "D3D11ColorShader.h"

namespace Soul
{
	D3D11ShaderManager::D3D11ShaderManager(D3D11Device& device)
		:
		mDevice(device)
	{
		ResourceGroupManager::GetInstance().RegisterResourceManager(L"D3DSShader", this);
	}

	D3D11ShaderManager::~D3D11ShaderManager()
	{
		ResourceGroupManager::GetInstance().UnregisterResourceManager(L"D3DSShader");
	}

	void D3D11ShaderManager::AddShader(const std::wstring& shaderName)
	{
		Shader* shader = nullptr;
		if (shaderName == L"Basic")
		{
			shader = new D3D11BasicShader(mDevice, shaderName, mShaderConfig[WstringToString(shaderName)]);
		}
		else if (shaderName == L"Texture2D")
		{
			shader = new D3D11TextureShader(mDevice, shaderName, mShaderConfig[WstringToString(shaderName)]);
		}
		else if (shaderName == L"Color")
		{
			shader = new D3D11ColorShader(mDevice, shaderName, mShaderConfig[WstringToString(shaderName)]);
		}
		else
		{
			return;
		}
		mShaderList.emplace(shaderName, shader);
	}
}
