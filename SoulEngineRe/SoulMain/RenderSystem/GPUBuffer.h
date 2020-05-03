#pragma once
namespace Soul
{
	enum class GPU_BUFFER_TYPE
	{
		GBT_VERTEX,
		GBT_INDEX,
		GBT_CONST
	};

	class GPUBuffer
	{
	public:
		GPUBuffer() {};
		virtual ~GPUBuffer() {};
	};
}