#pragma once
#include "D3D11Shader.h"
#include "../../SoulMain/Scene/Light.h"
#include "../../SoulMain/Scene/SubMesh.h"

namespace Soul
{
	struct BasicCBufferPerObj
	{
		Core::SMatrix4x4 world;
		Core::SMatrix4x4 view;
		Core::SMatrix4x4 proj;
		Material material;
		Core::SMatrix4x4 shadowMat;
		int shadow = 0;
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

	//struct BasicCBufferShadowRef
	//{
	//};

	class D3D11BasicShader : public D3D11Shader
	{
	public:
		D3D11BasicShader(D3D11Device& device, const std::wstring& shaderName, const json& shaderConfig)
			:
			D3D11Shader(device, shaderName, shaderConfig),
			mConstantBufferPerObj(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(BasicCBufferPerObj)),
			mConstantBufferPerFrame(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(BasicCBufferPerFrame))/*,
			mConstantBufferShadowRef(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(BasicCBufferShadowRef))*/
		{
		}
		void SetWorld(const Core::SMatrix4x4& world) override
		{
			mConstantsPerObj.world = world;
		}
		void SetView(const Core::SMatrix4x4& view) override
		{
			mConstantsPerObj.view = view;
		}
		void SetProj(const Core::SMatrix4x4& proj) override
		{
			mConstantsPerObj.proj = proj;
		}
		void SetShadowMatrix(const Core::SMatrix4x4& shadowMat) override
		{
			mConstantsPerObj.shadowMat = shadowMat;
		}
		void SetEnableShadow(bool useShadow) override
		{
			mConstantsPerObj.shadow = useShadow ? 1 : 0;
		};
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
			Core::MatrixTranspose(mConstantsPerObj.proj);
			Core::MatrixTranspose(mConstantsPerObj.view);
			Core::MatrixTranspose(mConstantsPerObj.world);
			if (mConstantsPerObj.shadow == 1)
			{
				Core::MatrixTranspose(mConstantsPerObj.shadowMat);
			}
			mConstantBufferPerObj.UpdateBuffer(&mConstantsPerObj, sizeof(mConstantsPerObj));
			mConstantBufferPerFrame.UpdateBuffer(&mConstantsPerFrame, sizeof(mConstantsPerFrame));
			// Set the constant buffer.
			ID3D11Buffer* buffer1 = mConstantBufferPerObj.GetD3D11Buffer();
			deviceContext->VSSetConstantBuffers(1, 1, &buffer1);
			deviceContext->PSSetConstantBuffers(1, 1, &buffer1);
			ID3D11Buffer* buffer2 = mConstantBufferPerFrame.GetD3D11Buffer();
			deviceContext->PSSetConstantBuffers(0, 1, &buffer2);
			//ID3D11Buffer* buffer3 = mConstantBufferShadowRef.GetD3D11Buffer();
			//deviceContext->VSSetConstantBuffers(2, 1, &buffer3);
		}
	private:
		//静态缓存对应的结构体变量
		BasicCBufferPerObj mConstantsPerObj;
		//静态缓存对应的结构体变量
		BasicCBufferPerFrame mConstantsPerFrame;
		//静态缓存对应的结构体变量
		//BasicCBufferShadowRef mBasicCBufferShadowRef;
		//静态缓存
		D3D11GPUBuffer mConstantBufferPerObj;
		//静态缓存
		D3D11GPUBuffer mConstantBufferPerFrame;
		//阴影反射
		//D3D11GPUBuffer mConstantBufferShadowRef;
	};
}