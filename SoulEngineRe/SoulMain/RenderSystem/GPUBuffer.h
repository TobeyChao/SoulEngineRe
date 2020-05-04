#pragma once
namespace Soul
{
	enum class GPU_BUFFER_TYPE
	{
		GBT_VERTEX,
		GBT_VERTEX_DYNAMIC,
		GBT_INDEX,
		GBT_CONSTANT
	};

	class GPUBuffer
	{
	public:
		GPUBuffer() {};
		virtual ~GPUBuffer() {};
		virtual void UpdateBuffer(const void* mem, unsigned byteSize) = 0;
	};
}