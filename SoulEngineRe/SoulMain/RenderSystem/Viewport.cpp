#include "Viewport.h"

#include "../Launcher.h"
#include "../Scene/SceneNodeCamera.h"
#include "RenderTarget.h"

namespace Soul
{
	Viewport::Viewport(SceneNodeCamera* camera, RenderTarget* renderTarget, float left, float top, float width, float height)
		:
		mCamera(camera),
		mRenderTarget(renderTarget),
		mViewportRelativeLeft(left),
		mViewportRelativeTop(top),
		mViewportRelativeWidth(width),
		mViewportRelativeHeight(height),
		mViewportLeft(0),
		mViewportTop(0),
		mViewportWidth(0),
		mViewportHeight(0)
	{
		mViewportLeft = int(mViewportRelativeLeft * float((mRenderTarget->GetWidth())));
		mViewportTop = int(mViewportRelativeTop * float((mRenderTarget->GetHeight())));
		mViewportWidth = int(mViewportRelativeWidth * float((mRenderTarget->GetWidth())));
		mViewportHeight = int(mViewportRelativeHeight * float((mRenderTarget->GetHeight())));
	}

	void Viewport::Update()
	{
		if (mCamera)
		{
			// Render Scene
			if (mCamera->GetViewport() != this)
				mCamera->SetViewport(this);
			mCamera->UpdateCamera();
			mCamera->RenderScene();
		}
	}
}