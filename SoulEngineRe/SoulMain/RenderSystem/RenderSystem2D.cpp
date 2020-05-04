#include "RenderSystem2D.h"

namespace Soul
{
	template<> RenderSystem2D* Singleton<RenderSystem2D>::mSingleton = nullptr;

	RenderSystem2D::RenderSystem2D()
	{
	}

	RenderSystem2D::~RenderSystem2D()
	{
	}
}