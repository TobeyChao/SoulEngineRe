#include "ShaderManager.h"
#include <fstream>

namespace Soul
{
	template<> ShaderManager* Singleton<ShaderManager>::mSingleton = nullptr;
	ShaderManager::ShaderManager()
	{
	}

	ShaderManager::~ShaderManager()
	{
		for (auto it : mShaderList)
		{
			Shader* shader = it.second;
			if (shader)
			{
				delete shader;
				shader = nullptr;
			}
		}
		mShaderList.clear();
	}

	void ShaderManager::SetJsonConfig(std::string configPath, bool loadAuto)
	{
		std::ifstream i(configPath);
		i >> mShaderConfig;
		i.close();
	}

	Shader* ShaderManager::GetShaderByName(const std::wstring& shaderName) const
	{
		auto it = mShaderList.find(shaderName);
		if (it != mShaderList.end())
		{
			return it->second;
		}
		return nullptr;
	}

}