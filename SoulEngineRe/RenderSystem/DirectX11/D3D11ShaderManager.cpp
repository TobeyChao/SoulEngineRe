#include "D3D11ShaderManager.h"
#include "../../SoulMain/Resource/ResourceGroupManager.h"

#include "D3D11SimpleShader.h"
#include "D3D11BasicShader.h"

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
		else
		{
			shader = new D3D11SimpleShader(mDevice, shaderName, mShaderConfig[WstringToString(shaderName)]);;
		}
		mShaderList.emplace(shaderName, shader);
	}
}
