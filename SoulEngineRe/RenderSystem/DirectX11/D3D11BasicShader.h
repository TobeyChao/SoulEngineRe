#pragma once
#include "D3D11Shader.h"
#include "../../SoulMain/Scene/Light.h"
#include "../../SoulMain/Scene/SubMesh.h"

namespace Soul
{
	struct BasicCBChangesEveryDrawing
	{
		Core::SMatrix4x4 world;
		Core::SMatrix4x4 view;
		Core::SMatrix4x4 proj;
		Material material;
	};

	struct BasicCBChangesEveryFrame
	{
		Core::SVector3 eyePosW;
		float pad1 = 0;
	};

	struct BasicCBChangesRarely
	{
		DirectionalLight dirLight[Light::MaxLights];
		PointLight pointLight[Light::MaxLights];
		SpotLight spotLight[Light::MaxLights];
		int numDirLight = 0;
		int numPointLight = 0;
		int numSpotLight = 0;
		float pad1 = 0;
		Core::SMatrix4x4 shadowMat;
		BOOL shadow = FALSE;
		BOOL useTexture = FALSE;
		BOOL useNormalMap = FALSE;
		float pad3 = 0;
		Core::SVector4 shadowPlane;
	};

	class D3D11BasicShader : public D3D11Shader
	{
	public:
		D3D11BasicShader(D3D11Device& device, const std::wstring& shaderName, const json& shaderConfig)
			:
			D3D11Shader(device, shaderName, shaderConfig),
			mConstantBufferChangesEveryDrawing(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(BasicCBChangesEveryDrawing)),
			mConstantBufferChangesEveryFrame(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(BasicCBChangesEveryFrame)),
			mConstantBufferChangesRarely(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(BasicCBChangesRarely))
		{
		}
		void SetWorld(const Core::SMatrix4x4& world) override
		{
			mBasicCBChangesEveryDrawing.world = world;
			Core::MatrixTranspose(mBasicCBChangesEveryDrawing.world);
			mIsDirty = mConstantBufferChangesEveryDrawing.mIsDirty = true;
		}
		void SetView(const Core::SMatrix4x4& view) override
		{
			mBasicCBChangesEveryDrawing.view = view;
			Core::MatrixTranspose(mBasicCBChangesEveryDrawing.view);
			mIsDirty = mConstantBufferChangesEveryDrawing.mIsDirty = true;
		}
		void SetProj(const Core::SMatrix4x4& proj) override
		{
			mBasicCBChangesEveryDrawing.proj = proj;
			Core::MatrixTranspose(mBasicCBChangesEveryDrawing.proj);
			mIsDirty = mConstantBufferChangesEveryDrawing.mIsDirty = true;
		}
		void SetShadowMatrix(const Core::SMatrix4x4& shadowMat) override
		{
			mBasicCBChangesRarely.shadowMat = shadowMat;
			Core::MatrixTranspose(mBasicCBChangesRarely.shadowMat);
		}
		void SetShadowPlane(const Core::SVector4& shadowPlane) override
		{
			mBasicCBChangesRarely.shadowPlane = shadowPlane;
		}
		void SetEnableShadow(bool useShadow) override
		{
			mBasicCBChangesRarely.shadow = useShadow ? 1 : 0;
		};
		void SetMaterial(const Material& matrial) override
		{
			mBasicCBChangesEveryDrawing.material = matrial;
			mIsDirty = mConstantBufferChangesEveryDrawing.mIsDirty = true;
		}
		void SetEyePos(const Core::SVector3& eyePos) override
		{
			mBasicCBChangesEveryFrame.eyePosW = eyePos;
			mIsDirty = mConstantBufferChangesEveryFrame.mIsDirty = true;
		}
		void SetDirectinalLight(int slot, const DirectionalLight& light) override
		{
			if (slot < Light::MaxLights && slot >= 0)
			{
				mBasicCBChangesRarely.dirLight[slot] = light;
				mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
			}
		};
		void SetSpotLight(int slot, const SpotLight& light) override
		{
			if (slot < Light::MaxLights && slot >= 0)
			{
				mBasicCBChangesRarely.spotLight[slot] = light;
				mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
			}
		};
		void SetPointLight(int slot, const PointLight& light) override
		{
			if (slot < Light::MaxLights && slot >= 0)
			{
				mBasicCBChangesRarely.pointLight[slot] = light;
				mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
			}
		};
		void SetDirectinalLightNum(int num) override
		{
			if (num <= Light::MaxLights && num >= 0)
			{
				mBasicCBChangesRarely.numDirLight = num;
				mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
			}
		};
		void SetSpotLightNum(int num) override
		{
			if (num <= Light::MaxLights && num >= 0)
			{
				mBasicCBChangesRarely.numSpotLight = num;
				mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
			}
		};
		void SetPointLightNum(int num) override
		{
			if (num <= Light::MaxLights && num >= 0)
			{
				mBasicCBChangesRarely.numPointLight = num;
				mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
			}
		};
		void SetUseTexture(bool useTexture) override
		{
			mBasicCBChangesRarely.useTexture = useTexture;
			mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
		};
		void SetUseNormalMap(bool useNormalMap) override
		{
			mBasicCBChangesRarely.useNormalMap = useNormalMap;
			mIsDirty = mConstantBufferChangesRarely.mIsDirty = true;
		};
		void ApplyShader() override
		{
			ID3D11DeviceContext* deviceContext = mDevice.GetDeviceContext();
			// Set the vertex input layout.
			deviceContext->IASetInputLayout(GetInputLayout());
			// Set the vertex and pixel shaders that will be used to render.
			deviceContext->VSSetShader(GetVertexShader(), nullptr, 0u);
			deviceContext->PSSetShader(GetPixelShader(), nullptr, 0u);
			// Set the constant buffer.
			if (mIsDirty)
			{
				mIsDirty = false;
				mConstantBufferChangesEveryDrawing.UpdateBuffer(&mBasicCBChangesEveryDrawing, sizeof(mBasicCBChangesEveryDrawing));
				mConstantBufferChangesEveryFrame.UpdateBuffer(&mBasicCBChangesEveryFrame, sizeof(mBasicCBChangesEveryFrame));
				mConstantBufferChangesRarely.UpdateBuffer(&mBasicCBChangesRarely, sizeof(mBasicCBChangesRarely));
			}
			mConstantBufferChangesEveryDrawing.BindPS(0);
			mConstantBufferChangesEveryDrawing.BindVS(0);
			mConstantBufferChangesEveryFrame.BindPS(1);
			mConstantBufferChangesRarely.BindPS(2);
			mConstantBufferChangesRarely.BindVS(2);
		}
	private:
		//静态缓存对应的结构体变量
		BasicCBChangesEveryDrawing mBasicCBChangesEveryDrawing;
		//静态缓存对应的结构体变量
		BasicCBChangesEveryFrame mBasicCBChangesEveryFrame;
		//静态缓存对应的结构体变量
		BasicCBChangesRarely mBasicCBChangesRarely;
		//静态缓存
		D3D11GPUBuffer mConstantBufferChangesEveryDrawing;
		//静态缓存
		D3D11GPUBuffer mConstantBufferChangesEveryFrame;
		//阴影反射
		D3D11GPUBuffer mConstantBufferChangesRarely;
	};
}