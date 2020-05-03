#pragma once
#include "Shader.h"
#include "../Resource/ResourceManager.h"
#include "../Utils/UtilsPCH.h"

namespace Soul
{
	enum ShaderManagerType
	{
		SHADER_MANAGER_TYPE_DX,
		SHADER_MANAGER_TYPE_GL,
	};
	class ShaderManager : public ResourceManager, public Singleton<ShaderManager>
	{
	public:
		ShaderManager();
		virtual ~ShaderManager();
		void SetJsonConfig(std::string configPath, bool loadAuto = false);
		virtual void AddShader(const std::wstring& shaderName) = 0;
		[[nodiscard]] virtual Shader* GetShaderByName(const std::wstring& shaderName) const;
	protected:
		typedef std::map<std::wstring, Shader*> ShaderMap;
		ShaderMap mShaderList;
		json mShaderConfig;
	};
}
