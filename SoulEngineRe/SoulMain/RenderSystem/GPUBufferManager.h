#pragma once
#include "../SoulPCH.h"
namespace Soul
{
	class GPUBufferManager : public Singleton<GPUBufferManager>
	{
	public:
		GPUBufferManager();
		virtual ~GPUBufferManager() = default;
		virtual GPUBuffer* CreateGPUBuffer(GPU_BUFFER_TYPE gpuBufferType, const void* mem, unsigned byteSize) = 0;
	};
}
