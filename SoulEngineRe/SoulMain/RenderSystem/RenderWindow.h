#pragma once
#include "../Utils/UtilsPCH.h"
#include "RenderTarget.h"
namespace Soul
{
	class RenderWindow : public RenderTarget
	{
	public:
		RenderWindow();
		virtual ~RenderWindow();

		virtual bool Initialize(const json& createParams) = 0;
		virtual void UnInitialize() = 0;

	protected:
		bool mIsFullScreen;
		int mLeft;
		int mTop;
	};
}