#pragma once
#include "GPUBuffer.h"
namespace Soul
{
	enum class PrimitiveTopology
	{
		PT_TRIANGLELIST,			// 三角形带
		PT_LINELIST,				// 线带
		PT_POINTLIST,				// 点带
	};

	enum class BlendType
	{
		BT_DEFAULT,
		BT_TRANSPARENT,	// 透明混合
		BT_ADD,			//颜色加法
	};

	enum class RasterizerType
	{
		RT_DEFAULT,
		RT_CULL_NONE,				// 不剔除
		RT_CULL_CLOCKWISE,			// 正面为顺时针绕线，剔除正面（顺时针）
		RT_CULL_COUNTERCLOCKWISE,	// 正面为顺时针绕线，剔除反面（逆时针）
		RT_WIREFRAME,				// 正面为顺时针绕线，线框模式，不剔除
	};

	enum class DepthStencilType
	{
		DST_DEFAULT,
		DST_LESS_EQUAL,		// 深度测试小于等于，用于天空盒
		DST_NO_DEPTH_WRITE,	// 深度测试开启，但是不写入深度值
		DST_NO_DOUBLE_BLEND,// 无二次混合
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