#pragma once
#include "../../SoulMain/RenderSystem/GPUBufferManager.h"
#include "D3D11Device.h"

namespace Soul
{
	class D3D11GPUBufferManager : public GPUBufferManager
	{
	public:
		D3D11GPUBufferManager(D3D11Device& device);
		GPUBuffer* CreateGPUBuffer(GPU_BUFFER_TYPE gpuBufferType, const void* mem, unsigned byteSize) override;
	private:
		D3D11Device& mDevice;
	};
}