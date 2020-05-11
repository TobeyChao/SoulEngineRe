#pragma once
#include "../Resource/Resource.h"
#include "../Core/Mathematics/SMatrix4x4.h"
#include "../Utils/UtilsPCH.h"
namespace Soul
{
	struct Material;
	struct DirectionalLight;
	struct PointLight;
	struct SpotLight;

	enum class SHADER_TYPE
	{
		ST_HLSL,
		ST_GLSL
	};
	class Shader : public Resource
	{
	public:
		Shader(enum class SHADER_TYPE shaderType, const std::wstring& shaderName, const json& shaderConfig)
			:
			Resource(shaderName),
			mShaderName(shaderName),
			mShaderType(shaderType),
			mShaderConfig(shaderConfig)
		{
		}
		virtual ~Shader()
		{
		}
		std::wstring GetShaderName() const
		{
			return mShaderName;
		}
		const json& GetShaderConfig() const
		{
			return mShaderConfig;
		}
		virtual void ApplyShader() = 0;
		virtual void SetWorldViewProj(const Core::SMatrix4x4& wvp) = 0;
		virtual void SetView(const Core::SMatrix4x4& view) = 0;
		virtual void SetProj(const Core::SMatrix4x4& proj) = 0;
		virtual void SetWorld(const Core::SMatrix4x4& world) = 0;
		virtual void SetShadowMatrix(const Core::SMatrix4x4& shadowMat) = 0;
		virtual void SetEnableShadow(bool useShadow) = 0;
		virtual void SetMaterial(const Material& matrial) = 0;
		virtual void SetEyePos(const Core::SVector3& eyePos) = 0;

		virtual void SetDirectinalLight(int slot, const DirectionalLight& light) = 0;
		virtual void SetSpotLight(int slot, const SpotLight& light) = 0;
		virtual void SetPointLight(int slot, const PointLight& light) = 0;

		virtual void SetDirectinalLightNum(int num) = 0;
		virtual void SetSpotLightNum(int num) = 0;
		virtual void SetPointLightNum(int num) = 0;
		virtual void SetUseTexture(bool useTexture) = 0;
	protected:
		std::wstring mShaderName;
		SHADER_TYPE mShaderType;
		const json& mShaderConfig;
	};
}