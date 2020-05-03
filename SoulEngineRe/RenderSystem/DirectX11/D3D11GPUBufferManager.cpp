#include "D3D11GPUBufferManager.h"
#include "D3D11GPUBuffer.h"

namespace Soul
{
	D3D11GPUBufferManager::D3D11GPUBufferManager(D3D11Device& device)
		:
		mDevice(device)
	{
	}
	GPUBuffer* D3D11GPUBufferManager::CreateGPUBuffer(GPU_BUFFER_TYPE gpuBufferType, const void* mem, unsigned byteSize)
	{
		GPUBuffer* buffer = new D3D11GPUBuffer(mDevice, gpuBufferType, mem, byteSize);
		return buffer;
	}
}