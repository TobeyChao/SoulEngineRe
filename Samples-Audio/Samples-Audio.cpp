#include "pch.h"
#include "../SoulEngineRe/RenderSystem/DirectX11/D3D11Texture.h"

class AudioSample final : public Soul::App::Application
{
public:
	~AudioSample()
	{
		delete sceneMgr;
	}

	// 初始化资源
	bool Setup() override
	{
		if (!Application::Setup())
		{
			return false;
		}

		Launcher* launcher = GetLauncher();
		sceneMgr = launcher->CreateSceneManager();
		mWindow->SetClearColor({ 0.0f, 0.2f, 0.4f, 1.0f });

#pragma region CameraAndRt
		camera = new SceneNodeCamera(sceneMgr, sceneMgr, 101);
		sceneMgr->AddChild(camera);
		mWindow->AddViewport(camera, 0, 0.0f, 0.0f, 1.f, 1.f);
#pragma endregion

		audio = SoundResourceManager::GetInstance().LoadSound(L"bgm", L"../Assets/sounds/卫兰+-+你的眼神.wav");

		return true;
	}

	// 每帧开始处理
	bool FrameStarted() override
	{
		if (Input::DXInput::GetInstance().IsPressed(DIK_ESCAPE))
		{
			return false;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_F1) &&
			!Input::DXInput::GetInstance().IsLastPressed(DIK_F1))
		{
			audio->Stop();
			state = L"停止";
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_F2) &&
			!Input::DXInput::GetInstance().IsLastPressed(DIK_F2))
		{
			audio->Play(100, true);
			state = L"播放";
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_F3) &&
			!Input::DXInput::GetInstance().IsLastPressed(DIK_F3))
		{
			audio->Pause();
			state = L"暂停";
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_F4) &&
			!Input::DXInput::GetInstance().IsLastPressed(DIK_F4))
		{
			audio->Resume();
			state = L"播放";
		}

		return Application::FrameStarted();
	}

	bool FrameUpdated() override
	{
		std::wostringstream buffer;
		buffer << L"按下F1停止" << std::endl
			<< L"按下F2播放" << std::endl
			<< L"按下F3暂停" << std::endl
			<< L"按下F4继续" << std::endl
			<< L"当前的播放进度：%" << int(audio->GetProgress() * 100) << std::endl
			<< L"当前的播放音量：" << audio->GetVolume() << std::endl
			<< L"当前的播放状态：" << state << std::endl;
		std::wstring info = buffer.str();

		RenderSystem2D::GetInstance().DrawTextW(info, { 10, 10 });
		return Application::FrameUpdated();
	}

	bool FrameEnd() override
	{
		return Application::FrameEnd();
	}

private:
	IAudioBuffer* audio;
	std::wstring state = L"未播放";
	SceneNodeCamera* camera{};
	SceneManager* sceneMgr{};
};

int main()
{
	AudioSample renderSample;
	if (!renderSample.Initialize())
	{
		return 1;
	}
	renderSample.GetLauncher()->Run();
	renderSample.UnInitialize();
	return 0;
}