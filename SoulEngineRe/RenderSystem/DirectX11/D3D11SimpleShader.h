#pragma once
#include "D3D11Shader.h"
namespace Soul
{
	struct WVPConstantBuffer
	{
		Core::SMatrix4x4 WVP;
	};

	class D3D11SimpleShader : public D3D11Shader
	{
	public:
		D3D11SimpleShader(D3D11Device& device, const std::wstring& shaderName, const json& shaderConfig)
			:
			D3D11Shader(device, shaderName, shaderConfig),
			mConstantBuffer(device, GPU_BUFFER_TYPE::GBT_CONSTANT, nullptr, sizeof(WVPConstantBuffer))
		{
		}
		void SetWorldViewProj(const Core::SMatrix4x4& wvp) override
		{
			mConstantsPerObj.WVP = wvp;
		}
		void ApplyShader() override
		{
			ID3D11DeviceContext* deviceContext = mDevice.GetDeviceContext();
			// Set the vertex input layout.
			deviceContext->IASetInputLayout(GetInputLayout());
			// Set the vertex and pixel shaders that will be used to render.
			deviceContext->VSSetShader(GetVertexShader(), nullptr, 0u);
			deviceContext->PSSetShader(GetPixelShader(), nullptr, 0u);
			Core::MatrixTranspose(mConstantsPerObj.WVP);
			mConstantBuffer.UpdateBuffer(&mConstantsPerObj, sizeof(mConstantsPerObj));
			// Set the constant buffer.
			ID3D11Buffer* buffer = mConstantBuffer.GetD3D11Buffer();
			deviceContext->VSSetConstantBuffers(0, 1, &buffer);
		}
	private:
		//静态缓存对应的结构体变量
		WVPConstantBuffer mConstantsPerObj;
		//静态缓存
		D3D11GPUBuffer mConstantBuffer;
	};
}