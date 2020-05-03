#pragma once
#include "GPUBuffer.h"
namespace Soul
{
	enum class PrimitiveTopology
	{
		PT_TRIANGLELIST,
		PT_LINELIST,
		PT_POINTLIST,
	};
	class RenderParameter
	{
	public:
		RenderParameter()
			:
			mPrimitiveTopology(PrimitiveTopology::PT_TRIANGLELIST),
			mSizePerVertex(0),
			mIndicesCount(0),
			mVertexCount(0),
			mVertexBuffer(nullptr),
			mIndexBuffer(nullptr)
		{
		}
		PrimitiveTopology mPrimitiveTopology;
		//Vertex Size
		unsigned int mSizePerVertex;
		//Index Count
		unsigned int mIndicesCount;
		//Vertex Count
		unsigned int mVertexCount;
		//Vertex Buffer
		GPUBuffer* mVertexBuffer;
		//Index Buffer
		GPUBuffer* mIndexBuffer;
	};
}