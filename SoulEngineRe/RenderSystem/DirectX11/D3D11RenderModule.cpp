#include "D3D11RenderModule.h"
#include "../../SoulMain/Launcher.h"
namespace Soul
{
	void D3D11RenderModule::Initialize()
	{
		mDx11RenderSystem = new D3D11RenderSystem();
		// Add the render system to admin
		Launcher::GetInstance().AddRenderSystem(mDx11RenderSystem);
	}

	void D3D11RenderModule::Uninitialize()
	{
		delete mDx11RenderSystem;
		mDx11RenderSystem = nullptr;
	}
}
