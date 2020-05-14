#pragma once
#include "../../SoulMain/RenderSystem/GPUBuffer.h"
#include "D3D11Device.h"

namespace Soul
{
	class D3D11GPUBuffer : public GPUBuffer
	{
	public:
		D3D11GPUBuffer(D3D11Device& device, enum class GPU_BUFFER_TYPE gpuBufferType, const void* mem, unsigned byteSize);

		[[nodiscard]] ID3D11Buffer* GetD3D11Buffer() const
		{
			return mD3D11Buffer.Get();
		}

		void UpdateBuffer(const void* mem, unsigned byteSize) override
		{
			//if (mIsDirty)
			{
				//mIsDirty = false;
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				mDevice.GetDeviceContext()->Map(mD3D11Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				//*static_cast<T*>(mappedResource.pData) = value;
				memcpy_s(mappedResource.pData, byteSize, mem, byteSize);
				mDevice.GetDeviceContext()->Unmap(mD3D11Buffer.Get(), 0);
			}
		}

		void BindPS(unsigned startSlot)
		{
			ID3D11DeviceContext* deviceContext = mDevice.GetDeviceContext();
			deviceContext->PSSetConstantBuffers(startSlot, 1, mD3D11Buffer.GetAddressOf());
		}

		void BindVS(unsigned startSlot)
		{
			ID3D11DeviceContext* deviceContext = mDevice.GetDeviceContext();
			deviceContext->VSSetConstantBuffers(startSlot, 1, mD3D11Buffer.GetAddressOf());
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mD3D11Buffer;
		D3D11Device& mDevice;
	};
}
