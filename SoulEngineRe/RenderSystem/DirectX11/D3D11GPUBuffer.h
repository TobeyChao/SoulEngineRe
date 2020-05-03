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
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mD3D11Buffer;
		D3D11Device& mDevice;
	};
}
