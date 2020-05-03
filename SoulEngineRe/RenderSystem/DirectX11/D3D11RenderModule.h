#pragma once
#include "../../SoulMain/Module/Module.h"
#include "D3D11RenderSystem.h"

namespace Soul
{
	class D3D11RenderModule : public Module
	{
	public:
		void Initialize() override;
		void Uninitialize() override;
	protected:
		D3D11RenderSystem* mDx11RenderSystem = nullptr;
	};
}