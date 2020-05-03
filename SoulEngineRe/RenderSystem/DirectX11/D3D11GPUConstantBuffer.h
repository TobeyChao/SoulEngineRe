#pragma once
#include "D3D11GPUBuffer.h"

namespace Soul
{
	template<typename T>
	class D3D11GPUConstantBuffer : public D3D11GPUBuffer
	{
	public:
		D3D11GPUConstantBuffer(D3D11Device& device)
			:
			D3D11GPUBuffer(device, GPU_BUFFER_TYPE::GBT_CONST, nullptr, sizeof(T))
		{
		}
		void UpdateBuffer(T const& value)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			mDevice.GetDeviceContext()->Map(mD3D11Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			*static_cast<T*>(mappedResource.pData) = value;
			//memcpy_s(mappedResource.pData, sizeof(T), &value, sizeof(T));
			mDevice.GetDeviceContext()->Unmap(mD3D11Buffer.Get(), 0);
		}
	};
}