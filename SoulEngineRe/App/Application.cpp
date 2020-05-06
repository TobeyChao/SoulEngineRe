#include "Application.h"
#include "../Platform/Win/WindowsUtils.h"
#include "../Platform/Win/Timer.h"
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
			mLauncher = nullptr;
			mCPUUse = new CPUUse();
		}

		Application::~Application()
		{
		}

		bool Application::Initialize()
		{
			mLauncher = new Launcher("../Assets/Config/boot_modules.json");
			return Setup();
		}

		void Application::UnInitialize()
		{
			mCPUUse->ShutDown();
			delete mCPUUse;
			Input::DXInput::GetInstance().Shutdown();
			delete mLauncher;
			mLauncher = nullptr;
		}

		bool Application::Setup()
		{
			mWindow = mLauncher->Initialize("../Assets/Config/boot_config.json");
			if (!mWindow)
			{
				return false;
			}
			mLauncher->AddFrameEventReceiver(this);
			D3D11RenderWindow* window = dynamic_cast<D3D11RenderWindow*>(mWindow);
			if (!window)
			{
				return false;
			}
			if (!Input::DXInput::GetInstance().Initialize(window->GetHWnd()))
			{
				return false;
			}

			mCPUUse->Initialize();
			
			Timer::InitTimer();
			Timer::Start();
			return true;
		}

		bool Application::FrameStarted()
		{
			// 更新键鼠状态
			Input::DXInput::GetInstance().Update();
			// 时间
			Timer::Tick();
			// CPU使用情况
			mCPUUse->Frame();
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

		int Application::GetCPUUSe()
		{
			return mCPUUse->GetCpuPerentage();
		}
	}
}