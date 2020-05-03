#include "Application.h"
#include "../Platform/Win/WindowsUtils.h"
#include "../Input/DXInput/Input.h"
#include "../RenderSystem/DirectX11/D3D11RenderWindow.h"

namespace Soul
{
	namespace App
	{
		Application::Application()
			:
			mWindow(nullptr)
		{
			mAdmin = nullptr;
		}

		Application::~Application()
		{
		}

		bool Application::Initialize()
		{
			mAdmin = new Launcher("../Assets/Config/boot_modules.json");
			return Setup();
		}

		void Application::UnInitialize()
		{
			Input::DXInput::GetInstance().Shutdown();
			delete mAdmin;
			mAdmin = nullptr;
		}

		bool Application::Setup()
		{
			mWindow = mAdmin->Initialize("../Assets/Config/boot_config.json");
			if (!mWindow)
			{
				return false;
			}
			mAdmin->AddFrameEventReceiver(this);
			D3D11RenderWindow* window = dynamic_cast<D3D11RenderWindow*>(mWindow);
			if (!window)
			{
				return false;
			}
			if (!Input::DXInput::GetInstance().Initialize(window->GetHWnd()))
			{
				return false;
			}
			return true;
		}

		bool Application::FrameStarted()
		{
			Input::DXInput::GetInstance().Update();
			// Process System Message
			bool res = Platform::WindowsUtils::MsgProcess();
			return res;
		}

		bool Application::FrameUpdated()
		{
			return true;
		}

		bool Application::FrameEnd()
		{
			return true;
		}
	}
}