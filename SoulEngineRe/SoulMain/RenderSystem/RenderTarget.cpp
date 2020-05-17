#include "RenderTarget.h"

namespace Soul
{
	RenderTarget::RenderTarget()
		:
		mClearColor({ 0.0f, 0.0f, 0.0f, 1.0f }),
		mWidth(0),
		mHeight(0)
	{
	}

	RenderTarget::~RenderTarget()
	{
		for (auto it : mViewports)
		{
			Viewport* viewport = it.second;
			if (viewport)
			{
				delete viewport;
				viewport = nullptr;
			}
		}
		mViewports.clear();
	}

	const std::string& RenderTarget::GetName() const
	{
		return mName;
	}

	unsigned int RenderTarget::GetWidth() const
	{
		return mWidth;
	}

	unsigned int RenderTarget::GetHeight() const
	{
		return mHeight;
	}

	Viewport* RenderTarget::AddViewport(SceneNodeCamera* camera, int ZOrder, float left, float top, float width, float height)
	{
		auto it = mViewports.find(ZOrder);
		if (it != mViewports.end())
		{
			return it->second;
		}
		auto viewport = new Viewport(camera, this, left, top, width, height);
		mViewports.emplace(ZOrder, viewport);
		return viewport;
	}

	void RenderTarget::Update()
	{
		Clear();
		for (auto it = mViewports.begin(); it != mViewports.end(); it++)
		{
			it->second->Update();
		}
	}
}