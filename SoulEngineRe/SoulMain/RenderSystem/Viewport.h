#pragma once
#include "../SoulPCH.h"
namespace Soul
{
	class Viewport
	{
	public:
		Viewport(SceneNodeCamera* camera, RenderTarget* renderTarget, float left, float top, float width, float height);
		RenderTarget* GetRenderTarget() const
		{
			return mRenderTarget;
		}
		void Update();
		int GetViewportLeft() const { return mViewportLeft; }
		int GetViewportTop() const { return mViewportTop; }
		int GetViewportWidth() const { return mViewportWidth; }
		int GetViewportHeight() const { return mViewportHeight; }
	private:
		SceneNodeCamera* mCamera;
		RenderTarget* mRenderTarget;
		float mViewportRelativeLeft;
		float mViewportRelativeTop;
		float mViewportRelativeWidth;
		float mViewportRelativeHeight;
		int mViewportLeft;
		int mViewportTop;
		int mViewportWidth;
		int mViewportHeight;
	};
}