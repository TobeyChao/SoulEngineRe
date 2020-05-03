#pragma once
#include <map>
#include <string>
#include "Viewport.h"

namespace Soul
{
	class RenderTarget
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();
		[[nodiscard]] virtual const std::string& GetName() const;

		[[nodiscard]] virtual unsigned int GetWidth() const;
		[[nodiscard]] virtual unsigned int GetHeight() const;
		virtual Viewport* AddViewport(SceneNodeCamera* camera, int ZOrder = 0, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
		virtual void Update();
		virtual void SwapBuffers() = 0;
	protected:
		unsigned int mWidth;
		unsigned int mHeight;
		std::string mName;
		std::map<int, Viewport*> mViewports;
	};
}