#pragma once
#include "D3D11Shader.h"
#include "../../SoulMain/Scene/Light.h"
#include "../../SoulMain/Scene/SubMesh.h"

namespace Soul
{
	struct BasicCBufferPerObj
	{
		Core::SMatrix4x4 world;
		Core::SMatrix4x4 wvp;
		Material material;
	};

	struct BasicCBufferPerFrame
	{
		DirectionalLight dirLight[Light::MaxLights];
		PointLight pointLight[Light::MaxLights];
		SpotLight spotLight[Light::MaxLights];
		int numDirLight = 0;
		int numPointLight = 0;
		int numSpotLight = 0;
		BOOL useTexture = FALSE;
		Core::SVector3 eyePosW;
		float pad2 = 0;
	};

	class D3D11BasicShader : public D3D11Shader
	{
	public:
		D3D11BasicShader(D3D11Device& device, const std::wstring& shaderName, const json& shaderConfig)
			:
			D3D11Shader(device, shaderName, shaderConfig),
			mConstantBufferPerObj(device),
			mConstantBufferPerFrame(device)
		{
		}
		void SetWorld(const Core::SMatrix4x4& world) override
		{
			mConstantsPerObj.world = world;
		}
		void SetWorldViewProj(const Core::SMatrix4x4& wvp) override
		{
			mConstantsPerObj.wvp = wvp;
		}
		void SetMaterial(const Material& matrial) override
		{
			mConstantsPerObj.material = matrial;
		}
		void SetEyePos(const Core::SVector3& eyePos) override
		{
			mConstantsPerFrame.eyePosW = eyePos;
		}
		void SetDirectinalLight(int slot, const DirectionalLight& light) override
		{
			if (slot < Light::MaxLights && slot >= 0)
			{
				mConstantsPerFrame.dirLight[slot] = light;
			}
		};
		void SetSpotLight(int slot, const SpotLight& light) override
		{
			if (slot < Light::MaxLights && slot >= 0)
				mConstantsPerFrame.spotLight[slot] = light;
		};
		void SetPointLight(int slot, const PointLight& light) override
		{
			if (slot < Light::MaxLights && slot >= 0)
				mConstantsPerFrame.pointLight[slot] = light;
		};
		void SetDirectinalLightNum(int num) override
		{
			if (num <= Light::MaxLights && num >= 0)
				mConstantsPerFrame.numDirLight = num;
		};
		void SetSpotLightNum(int num) override
		{
			if (num <= Light::MaxLights && num >= 0)
				mConstantsPerFrame.numSpotLight = num;
		};
		void SetPointLightNum(int num) override
		{
			if (num <= Light::MaxLights && num >= 0)
				mConstantsPerFrame.numPointLight = num;
		};
		void SetUseTexture(bool useTexture) override
		{
			mConstantsPerFrame.useTexture = useTexture;
		};
		void ApplyShader() override
		{
			ID3D11DeviceContext* deviceContext = mDevice.GetDeviceContext();
			// Set the vertex input layout.
			deviceContext->IASetInputLayout(GetInputLayout());
			// Set the vertex and pixel shaders that will be used to render.
			deviceContext->VSSetShader(GetVertexShader(), nullptr, 0u);
			deviceContext->PSSetShader(GetPixelShader(), nullptr, 0u);
			Core::MatrixTranspose(mConstantsPerObj.wvp);
			Core::MatrixTranspose(mConstantsPerObj.world);
			mConstantBufferPerObj.UpdateBuffer(mConstantsPerObj);
			mConstantBufferPerFrame.UpdateBuffer(mConstantsPerFrame);
			// Set the constant buffer.
			ID3D11Buffer* buffer = mConstantBufferPerObj.GetD3D11Buffer();
			deviceContext->VSSetConstantBuffers(1, 1, &buffer);
			deviceContext->PSSetConstantBuffers(1, 1, &buffer);
			buffer = mConstantBufferPerFrame.GetD3D11Buffer();
			deviceContext->PSSetConstantBuffers(0, 1, &buffer);
		}
	private:
		//静态缓存对应的结构体变量
		BasicCBufferPerObj mConstantsPerObj;
		//静态缓存对应的结构体变量
		BasicCBufferPerFrame mConstantsPerFrame;
		//静态缓存
		D3D11GPUConstantBuffer<BasicCBufferPerObj> mConstantBufferPerObj;
		//静态缓存
		D3D11GPUConstantBuffer<BasicCBufferPerFrame> mConstantBufferPerFrame;
	};
}