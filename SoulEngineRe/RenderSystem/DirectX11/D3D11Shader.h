#pragma once
#include "../../SoulMain/RenderSystem/Shader.h"
#include "../../SoulMain/RenderSystem/GPUBufferManager.h"
#include "../../SoulMain/Core/Mathematics/SMatrix4x4.h"
#include "D3D11Device.h"
#include "D3D11GPUBuffer.h"
#include "D3D11InputLayouts.h"

namespace Soul
{
	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(D3D11Device& device, const std::wstring& shaderName, const json& shaderConfig)
			:
			Shader(SHADER_TYPE::ST_HLSL, shaderName, shaderConfig),
			mVertexShader(nullptr),
			mPixelShader(nullptr),
			mVertexShaderBuffer(nullptr),
			mPixelShaderBuffer(nullptr),
			mLayout(nullptr),
			mSampleState(nullptr),
			mDevice(device),
			mIsDirty(false)
		{
			HRESULT result;
			ID3D10Blob* errorMessage = nullptr;
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
			// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
			// Setting this flag improves the shader debugging experience, but still allows 
			// the shaders to be optimized and to run exactly the way they will run in 
			// the release configuration of this program.
			dwShaderFlags |= D3DCOMPILE_DEBUG;
			// 在Debug环境下禁用优化以避免出现一些不合理的情况
			dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
			std::string path = mShaderConfig["path"];
			std::wstring shaderFilePath = StringToWstring(path);
			// Compile the vertex shader code.
			result = D3DCompileFromFile(shaderFilePath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", dwShaderFlags, 0,
				&mVertexShaderBuffer, &errorMessage);
			result = D3DCompileFromFile(shaderFilePath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", dwShaderFlags, 0,
				&mPixelShaderBuffer, &errorMessage);
			mDevice->CreateVertexShader(mVertexShaderBuffer->GetBufferPointer(),
				mVertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader);
			mDevice->CreatePixelShader(mPixelShaderBuffer->GetBufferPointer(),
				mPixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader);

			if (mShaderConfig["input_layout"] == "pos")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPos) / sizeof(InputLayouts::inputLayoutPos[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPos, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
			else if (mShaderConfig["input_layout"] == "pos_tex")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPosTex) / sizeof(InputLayouts::inputLayoutPosTex[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPosTex, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
			else if (mShaderConfig["input_layout"] == "pos_col")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPosCol) / sizeof(InputLayouts::inputLayoutPosCol[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPosCol, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
			else if (mShaderConfig["input_layout"] == "pos_normal_col")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPosNorCol) / sizeof(InputLayouts::inputLayoutPosNorCol[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPosNorCol, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
			else if (mShaderConfig["input_layout"] == "pos_tex_normal_col")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPosTexNorCol) / sizeof(InputLayouts::inputLayoutPosTexNorCol[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPosTexNorCol, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
			else if (mShaderConfig["input_layout"] == "pos_tex_col")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPosTexCol) / sizeof(InputLayouts::inputLayoutPosTexCol[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPosTexCol, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
			else if (mShaderConfig["input_layout"] == "pos_nor_tan_tex_col")
			{
				UINT numElements = sizeof(InputLayouts::inputLayoutPosNorTanTexCol) / sizeof(InputLayouts::inputLayoutPosNorTanTexCol[0]);
				mDevice->CreateInputLayout(InputLayouts::inputLayoutPosNorTanTexCol, numElements, mVertexShaderBuffer->GetBufferPointer(),
					mVertexShaderBuffer->GetBufferSize(), &mLayout);
			}
		}
		~D3D11Shader()
		{
		}
		void SetWorldViewProj(const Core::SMatrix4x4& wvp) override {};
		void SetWorld(const Core::SMatrix4x4& world) override {};
		void SetView(const Core::SMatrix4x4& view) override {};
		void SetProj(const Core::SMatrix4x4& proj) override {};
		void SetShadowMatrix(const Core::SMatrix4x4& shadowMat) override {};
		void SetShadowPlane(const Core::SVector4& shadowPlane) override {};
		void SetEnableShadow(bool useShadow) override {};
		void SetMaterial(const Material& matrial) override {};
		void SetEyePos(const Core::SVector3& eyePos) override {};
		void SetDirectinalLight(int slot, const DirectionalLight& light) override {};
		void SetSpotLight(int slot, const SpotLight& light) override {};
		void SetPointLight(int slot, const PointLight& light) override {};
		void SetDirectinalLightNum(int num) override {};
		void SetSpotLightNum(int num) override {};
		void SetPointLightNum(int num) override {};
		void SetUseTexture(bool useTexture) override {};
		void SetUseNormalMap(bool useNormalMap) override {};
		void SetFogState(bool isOn) override {};
		void SetFogStart(float fogStart) override {};
		void SetFogColor(const Core::SVector4& fogColor) override {};
		void SetFogRange(float fogRange) override {};
		ID3D11VertexShader* GetVertexShader() const
		{
			return mVertexShader;
		}
		ID3D11PixelShader* GetPixelShader() const
		{
			return mPixelShader;
		}
		ID3D11InputLayout* GetInputLayout() const
		{
			return mLayout;
		}
		ID3D11SamplerState* GetSampleState() const
		{
			return mSampleState;
		}
	protected:
		//顶点着色器
		ID3D11VertexShader* mVertexShader;
		//像素着色器
		ID3D11PixelShader* mPixelShader;
		//顶点着色器缓存
		ID3D10Blob* mVertexShaderBuffer;
		//像素着色器缓存
		ID3D10Blob* mPixelShaderBuffer;
		//输入布局
		ID3D11InputLayout* mLayout;
		//采样器
		ID3D11SamplerState* mSampleState;
		//Device
		D3D11Device& mDevice;
		//是否需要更新
		bool mIsDirty;
	};
}