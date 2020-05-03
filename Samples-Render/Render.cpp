#include "pch.h"

class RenderSample final : public Soul::App::Application
{
public:
	~RenderSample()
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

		Launcher* admin = GetAdmin();
		// SceneManager也是根节点
		sceneMgr = admin->CreateSceneManager();

		cube = sceneMgr->CreateGameObject("cube", SIMPLE_GAMEOBJECT::SG_CUBE);
		cube->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/braynzar.jpg"));

		plane = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE);
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg"));

		sphere = sceneMgr->CreateGameObject("sphere", SIMPLE_GAMEOBJECT::SG_SPHERE);
		sphere->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/Redocn_2013122305390429.jpg"));

		mesh = sceneMgr->CreateGameObject("sphere", L"../Assets/Models/box/box.obj");

		pointLight = sceneMgr->CreateLight("main light", LIGHT_TYPE::LT_POINT);
		pointLight->SetAmbient({ 0.3f, 0.3f, 0.3f, 0.3f });
		pointLight->SetDiffuse({ 1.f, 1.f, 1.f, 1.0f });
		pointLight->SetSpecular({ 0.7f, 0.7f, 0.7f, 1.0f });
		pointLight->SetAtt({ 0.0f, 0.5f, 0.0f });
		pointLight->SetRange(25.0f);
		pointLight->SetPosition({ 0.0f, 0.0f, -5.0f });

		spotLight1 = sceneMgr->CreateLight("spot light1", LIGHT_TYPE::LT_SPOT);
		spotLight1->SetPosition({ 2.0f, 0.0f, 2.0f });
		spotLight1->SetDirection({ 0.0f, -1.0f, 0.0f });
		spotLight1->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
		spotLight1->SetDiffuse({ 1.0f, 0.0f, 0.0f, 1.0f });
		spotLight1->SetSpecular({ 1.0f, 1.0f, 1.0f, 1.0f });
		spotLight1->SetAtt({ 1.0f, 0.0f, 0.0f });
		spotLight1->SetSpot(10.0f);
		spotLight1->SetRange(100.0f);

		spotLight2 = sceneMgr->CreateLight("spot light2", LIGHT_TYPE::LT_SPOT);
		spotLight2->SetPosition({ -2.0f, 0.0f, 2.0f });
		spotLight2->SetDirection({ 0.0f, -1.0f, 0.0f });
		spotLight2->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
		spotLight2->SetDiffuse({ 0.0f, 1.0f, 0.0f, 1.0f });
		spotLight2->SetSpecular({ 1.0f, 1.0f, 1.0f, 1.0f });
		spotLight2->SetAtt({ 1.0f, 0.0f, 0.0f });
		spotLight2->SetSpot(10.0f);
		spotLight2->SetRange(100.0f);

		spotLight3 = sceneMgr->CreateLight("spot light3", LIGHT_TYPE::LT_SPOT);
		spotLight3->SetPosition({ 0.0f, 0.0f, -2.0f });
		spotLight3->SetDirection({ 0.0f, -1.0f, 0.0f });
		spotLight3->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
		spotLight3->SetDiffuse({ 0.0f, 0.0f, 1.0f, 1.0f });
		spotLight3->SetSpecular({ 1.0f, 1.0f, 1.0f, 1.0f });
		spotLight3->SetAtt({ 1.0f, 0.0f, 0.0f });
		spotLight3->SetSpot(10.0f);
		spotLight3->SetRange(100.0f);

		dirLight = sceneMgr->CreateLight("dirLight", LIGHT_TYPE::LT_DIRECTIONAL);
		dirLight->SetAmbient({ 0.2f, 0.2f, 0.2f, 1.0f });
		dirLight->SetDiffuse({ 0.8f, 0.8f, 0.8f, 1.0f });
		dirLight->SetSpecular({ 0.5f, 0.5f, 0.5f, 1.0f });
		dirLight->SetDirection({ -0.577f, -0.577f, 0.577f });

		node = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		node->SetPosition({ 0.f, 0.f, 0.f });
		//node->SetScale({ 0.1f, 0.1f, 0.1f });
		node->AttachObj(mesh);

		node2 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		node2->AttachObj(plane);
		node2->SetPosition({ 0.f, -6.f, 0.f });
		node2->SetScale({ 10.0f, 10.0f, 10.0f });

		node3 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		node3->AttachObj(sphere);
		node3->SetPosition({ 2.f, 0.f, 0.f });

		lightNode = sceneMgr->AddChild(new SceneNodeLight(sceneMgr, sceneMgr));
		lightNode->AttachObj(dirLight);
		//lightNode->AttachObj(pointLight);
		lightNode->AttachObj(spotLight1);
		lightNode->AttachObj(spotLight2);
		lightNode->AttachObj(spotLight3);

		camera = sceneMgr->AddCameraSceneNode();
		camera->SetIsOrthogonal(false);
		mWindow->AddViewport(camera);

		return true;
	}

	// 每帧开始处理
	bool FrameStarted() override
	{
		if (Input::DXInput::GetInstance().IsPressed(DIK_ESCAPE))
		{
			return false;
		}

		sphereRotate.y += 0.03f;

		if (Input::DXInput::GetInstance().IsPressed(DIK_S))
		{
			pos.z -= 0.03f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_W))
		{
			pos.z += 0.03f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_D))
		{
			pos.x += 0.03f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_A))
		{
			pos.x -= 0.03f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_Q))
		{
			pos.y += 0.03f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_E))
		{
			pos.y -= 0.03f;
		}

		if (Input::DXInput::GetInstance().IsPressed(DIK_R))
		{
			rotate.x = rotate.y = rotate.z = 0;
			pos.x = pos.y = pos.z = 0;
		}

		if (Input::DXInput::GetInstance().IsPressed(DIK_1))
		{
			cameraChoose = 1;
		}
		else if (Input::DXInput::GetInstance().IsPressed(DIK_2))
		{
			cameraChoose = 2;
		}


		if ((Input::DXInput::GetInstance().GetMouseState().lX != Input::DXInput::GetInstance().GetLastMouseState().lX) ||
			(Input::DXInput::GetInstance().GetMouseState().lY != Input::DXInput::GetInstance().GetLastMouseState().lY))
		{
			cameraRotate.x += Input::DXInput::GetInstance().GetMouseState().lY * 0.001f;
			cameraRotate.y += Input::DXInput::GetInstance().GetMouseState().lX * 0.001f;

			if (cameraRotate.x > Core::SM_PIDIV4)
				cameraRotate.x = Core::SM_PIDIV4;
			if (cameraRotate.x < -Core::SM_PIDIV4)
				cameraRotate.x = -Core::SM_PIDIV4;
		}
		if (cameraChoose == 1)
		{
			ProcessFreeLookCamera();
		}
		else if (cameraChoose == 2)
		{
			ProcessThirdCamera();
		}

		char chInput[512];
		sprintf_s(chInput, "PosX:%f PosY:%f PosZ:%f\n", pos.x, pos.y, pos.z);
		OutputDebugString(chInput);

		camera->SetPosition(cameraPos);
		camera->SetRotation(cameraRotate);

		node->SetRotation(rotate);
		node->SetPosition(pos);

		node3->SetRotation(sphereRotate);
		return Application::FrameStarted();
	}

	// 第一人称相机
	void ProcessFreeLookCamera()
	{
		const Core::SVector3& forward = camera->GetForward();
		const Core::SVector3& right = camera->GetRight();
		if (Input::DXInput::GetInstance().IsPressed(DIK_UP))
		{
			cameraPos.x += 0.03f * forward.x;
			cameraPos.y += 0.03f * forward.y;
			cameraPos.z += 0.03f * forward.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_DOWN))
		{
			cameraPos.x -= 0.03f * forward.x;
			cameraPos.y -= 0.03f * forward.y;
			cameraPos.z -= 0.03f * forward.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_LEFT))
		{
			cameraPos.x -= 0.03f * right.x;
			cameraPos.y -= 0.03f * right.y;
			cameraPos.z -= 0.03f * right.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_RIGHT))
		{
			cameraPos.x += 0.03f * right.x;
			cameraPos.y += 0.03f * right.y;
			cameraPos.z += 0.03f * right.z;
		}
	}

	// 第三人称相机
	void ProcessThirdCamera()
	{
		float x = node->GetPosition().x + -5 * cosf(cameraRotate.x) * sinf(cameraRotate.y);
		float z = node->GetPosition().z + -5 * cosf(cameraRotate.x) * cosf(cameraRotate.y);
		float y = node->GetPosition().y + 5 * sinf(cameraRotate.x);
		cameraPos = { x, y, z };
	}

private:
	SceneNode* lightNode;
	SceneNode* node;
	SceneNode* node2;
	SceneNode* node3;
	Light* pointLight;
	Light* spotLight1;
	Light* spotLight2;
	Light* spotLight3;
	Light* dirLight;
	GameObject* cube;
	GameObject* sphere;
	GameObject* plane;
	GameObject* mesh;
	SceneNodeCamera* camera;
	SceneManager* sceneMgr;
	Core::SVector3 pos;
	Core::SVector3 rotate;
	Core::SVector3 sphereRotate;
	Core::SVector3 cameraPos = { 0.f, 0.f, -5.f };
	Core::SVector3 cameraRotate = { 0.f, 0.f, 0.f };
	int cameraChoose = 1;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	RenderSample renderSample;
	if (!renderSample.Initialize())
	{
		return 1;
	}
	renderSample.GetAdmin()->Run();
	renderSample.UnInitialize();
	return 0;
}