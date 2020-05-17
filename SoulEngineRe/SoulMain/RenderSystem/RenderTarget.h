#pragma once
#include <map>
#include <string>
#include "Viewport.h"
#include "../Core/SColorf.h"

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

		void SetClearColor(const Core::SColorf& color)
		{
			mClearColor = color;
		}

		virtual Viewport* AddViewport(SceneNodeCamera* camera, int ZOrder = 0, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
		virtual void Update();
		virtual void Clear() = 0;
		virtual void SwapBuffers() = 0;
	protected:
		Core::SColorf mClearColor;
		unsigned int mWidth;
		unsigned int mHeight;
		std::string mName;
		std::map<int, Viewport*> mViewports;
	};
}