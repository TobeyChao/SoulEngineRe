#include "pch.h"

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

		// Mesh
		mesh = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/house2/house2.obj", json());
		nodeMesh = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh->SetPosition({ 0.f, 0.f, 0.f });
		nodeMesh->SetScale({ 0.5f, 0.5f, 0.5f });
		nodeMesh->AttachObj(mesh);

		mesh2 = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/android/android.obj", json());
		nodeMesh2 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh2->SetPosition({ 5.f, 0.f, 0.f });
		nodeMesh2->SetScale({ 5.5f, 5.5f, 5.5f });
		nodeMesh2->AttachObj(mesh2);

		mesh3 = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/alita/alita.obj", json());
		nodeMesh3 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh3->SetPosition({ -5.f, 0.f, 0.f });
		nodeMesh3->SetScale({ 0.05f, 0.05f, 0.05f });
		nodeMesh3->AttachObj(mesh3);

		mesh4 = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/lucy/Lucy.obj", json());
		nodeMesh4 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh4->SetPosition({ -10.f, 0.f, 0.f });
		nodeMesh4->SetScale({ 0.005f, 0.005f, 0.005f });
		nodeMesh4->AttachObj(mesh4);

#pragma region Light
		dirLight = sceneMgr->CreateLight("dirLight", LIGHT_TYPE::LT_DIRECTIONAL);
		dirLight->SetAmbient({ 0.8f, 0.8f, 0.8f, 1.0f });
		dirLight->SetDiffuse({ 1.f, 1.f, 1.f, 1.0f });
		dirLight->SetSpecular({ 0.5f, 0.5f, 0.5f, 16.0f });
		dirLight->SetDirection({ 0.f, 0.f, 1.f });

		lightNode = sceneMgr->AddChild(new SceneNodeLight(sceneMgr, sceneMgr));
		lightNode->AttachObj(dirLight);
#pragma endregion


#pragma region CameraAndRt
		camera = new SceneNodeCamera(sceneMgr, sceneMgr, 101);
		sceneMgr->AddChild(camera);

		mWindow->AddViewport(camera, 0, 0.0f, 0.0f, 1.f, 1.f);
#pragma endregion
		return true;
	}

	// 每帧开始处理
	bool FrameStarted() override
	{
		if (Input::DXInput::GetInstance().IsPressed(DIK_R))
		{
			meshRotate.y += Timer::DeltaTime();
		}

		nodeMesh->SetRotation(meshRotate);
		nodeMesh4->SetRotation(meshRotate);
		nodeMesh3->SetRotation(meshRotate);
		nodeMesh2->SetRotation(meshRotate);

		if (Input::DXInput::GetInstance().IsPressed(DIK_ESCAPE))
		{
			return false;
		}

		const Core::SVector3& forward = camera->GetForward();
		const Core::SVector3& right = camera->GetRight();
		if (Input::DXInput::GetInstance().IsPressed(DIK_W))
		{
			cameraPos.x += 0.05f * forward.x;
			cameraPos.y += 0.05f * forward.y;
			cameraPos.z += 0.05f * forward.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_S))
		{
			cameraPos.x -= 0.05f * forward.x;
			cameraPos.y -= 0.05f * forward.y;
			cameraPos.z -= 0.05f * forward.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_A))
		{
			cameraPos.x -= 0.05f * right.x;
			cameraPos.y -= 0.05f * right.y;
			cameraPos.z -= 0.05f * right.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_D))
		{
			cameraPos.x += 0.05f * right.x;
			cameraPos.y += 0.05f * right.y;
			cameraPos.z += 0.05f * right.z;
		}
		camera->SetPosition(cameraPos);
		camera->SetRotation(cameraRotate);

		return Application::FrameStarted();
	}

	bool FrameUpdated() override
	{
		const Core::SVector2 pos = Input::DXInput::GetInstance().GetMouseLocation();
		Ray ray = Ray::ScreenToRay(*camera, pos);

		std::wostringstream buffer;
		buffer << L"CameraPos：(" << cameraPos.x << "," << cameraPos.y << "," << cameraPos.z << ")" << std::endl;

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

	SceneNode* nodeMesh{};
	GameObject* mesh{};

	SceneNode* nodeMesh2{};
	GameObject* mesh2{};

	SceneNode* nodeMesh3{};
	GameObject* mesh3{};

	SceneNode* nodeMesh4{};
	GameObject* mesh4{};

	SceneNode* lightNode{};
	Light* dirLight{};

	SceneNodeCamera* secondCamera{};

	SceneNodeCamera* camera{};
	Core::SVector3 cameraPos = { -10.f, 2.f, -5.f };
	Core::SVector3 cameraRotate = { 0.f, 0.f, 0.f };

	Core::SVector3 meshRotate = { 0.f, 0.f, 0.f };

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