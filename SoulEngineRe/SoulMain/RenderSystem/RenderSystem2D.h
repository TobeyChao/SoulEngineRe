#pragma once
#include "../SoulPCH.h"
#include "../Core/Mathematics/SVector2.h"

namespace Soul
{
	class RenderSystem2D : public Singleton<RenderSystem2D>
	{
	public:
		RenderSystem2D();
		virtual ~RenderSystem2D();
		virtual bool Initialize() = 0;
		virtual void DrawTextW(const std::wstring& text, const Core::SVector2& pos) = 0;
	};
}