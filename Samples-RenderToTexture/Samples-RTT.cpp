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

		ITexture* tex = TextureManager::GetInstance().CreateTexture(L"rtt", { 1024, 1024 }, false);

		// nodePlane
		json planeSet;
		planeSet["width"] = 20.0f;
		planeSet["depth"] = 20.0f;
		planeSet["m"] = 5.0f;
		planeSet["n"] = 5.0f;
		planeSet["maxU"] = 5.0f;
		planeSet["maxV"] = 5.0f;
		plane = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE, planeSet);
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg"));
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg_NORM.jpg"));
		nodePlane = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodePlane->AttachObj(plane);
		nodePlane->SetPosition({ 0.f, 0.f, 0.f });

		planeSet["maxU"] = 1.0f;
		planeSet["maxV"] = 1.0f;
		planeRtt = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE, planeSet);
		planeRtt->GetSubMesh(0)->PushTexture(tex);
		nodePlaneRtt = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodePlaneRtt->AttachObj(planeRtt);
		nodePlaneRtt->SetPosition({ 0.f, 10.f, 10.f });
		nodePlaneRtt->SetRotation({ -Core::SM_PIDIV2, 0.f, 0.f });

		// Cylinder
		json cylinderSet;
		cylinderSet["Shader"] = "Basic";
		cylinderSet["bottomRadius"] = 1.0f;
		cylinderSet["topRadius"] = 1.0f;
		cylinderSet["height"] = 5.0f;
		cylinderSet["sliceCount"] = 20;
		cylinderSet["stackCount"] = 10;

		cylinder = sceneMgr->CreateGameObject("cylinder", SIMPLE_GAMEOBJECT::SG_CYLINDER, cylinderSet);
		cylinder->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/bricks.dds"));
		cylinder->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/bricks_NORM.png"));
		nodeCylinder = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeCylinder->AttachObj(cylinder);
		nodeCylinder->SetPosition({ 4.f, 2.51f, 2.f });

		// Sphere
		json sphereSet;
		sphere = sceneMgr->CreateGameObject("sphere", SIMPLE_GAMEOBJECT::SG_SPHERE, sphereSet);
		sphere->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/stones.dds"));
		sphere->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/stones_NORM.png"));
		nodeSphere = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeSphere->AttachObj(sphere);
		nodeSphere->SetPosition({ -4.f, 1.51f, 2.f });

		// Cube
		json cubeSet;
		cube = sceneMgr->CreateGameObject("Cube", SIMPLE_GAMEOBJECT::SG_CUBE, cubeSet);
		cube->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/stones.dds"));
		cube->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/stones_NORM.png"));
		nodeCube = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeCube->AttachObj(cube);
		nodeCube->SetPosition({ 0.f, 0.51f, -4.f });

		// Mesh
		mesh = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/house2/house2.obj", json());
		nodeMesh = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh->SetPosition(sphereRotation);
		nodeMesh->SetScale({ 0.5f, 0.5f, 0.5f });
		nodeMesh->AttachObj(mesh);

#pragma region Light
		dirLight = sceneMgr->CreateLight("dirLight", LIGHT_TYPE::LT_DIRECTIONAL);
		dirLight->SetAmbient({ 1.f, 1.f, 1.f, 1.0f });
		dirLight->SetDiffuse({ 1.f, 1.f, 1.f, 1.0f });
		dirLight->SetSpecular({ 0.5f, 0.5f, 0.5f, 1.0f });
		dirLight->SetDirection({ -0.577f, -0.577f, 0.577f });

		lightNode = sceneMgr->AddChild(new SceneNodeLight(sceneMgr, sceneMgr));
		lightNode->AttachObj(dirLight);
#pragma endregion


#pragma region CameraAndRt
		camera = new SceneNodeCamera(sceneMgr, sceneMgr, 101);
		secondCamera = new SceneNodeCamera(sceneMgr, sceneMgr, 102);
		secondCamera->SetPosition({ 0.0f, 10.0f, -10.0f });
		secondCamera->SetRotation({ Core::SM_PIDIV4, 0.f, 0.f });
		sceneMgr->AddChild(camera);
		sceneMgr->AddChild(secondCamera);

		
		D3D11RenderTexture* rtt = dynamic_cast<D3D11RenderTexture*>(tex);
		rtt->AddViewport(secondCamera, 1, 0.0f, 0.0f, 1.f, 1.f);
		rtt->SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		GetLauncher()->GetActiveRenderSystem()->AddRenderTarget(rtt);

		mWindow->AddViewport(camera, 0, 0.0f, 0.0f, 1.f, 1.f);
#pragma endregion
		return true;
	}

	// 每帧开始处理
	bool FrameStarted() override
	{
		if (Input::DXInput::GetInstance().IsPressed(DIK_ESCAPE))
		{
			return false;
		}

		if (Input::DXInput::GetInstance().IsPressed(DIK_DOWN))
		{
			sphereRotation.z -= 0.1f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_UP))
		{
			sphereRotation.z += 0.1f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_RIGHT))
		{
			sphereRotation.x += 0.1f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_LEFT))
		{
			sphereRotation.x -= 0.1f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_PGUP))
		{
			sphereRotation.y += 0.1f;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_PGDN))
		{
			sphereRotation.y -= 0.1f;
		}

		nodeMesh->SetPosition(sphereRotation);

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
		buffer << L"鼠标位置：(" << pos.x << "," << pos.y << ")" << std::endl
			<< L"射线方向：(" << ray.mDirection.x << "," << ray.mDirection.y << "," << ray.mDirection.z << ")" << std::endl;;
		float dist = 0.0f;

		if (ray.Hit(plane->GetBoundingBox(), &dist))
		{
			buffer << L"射线与平面交叉，距离为：" << dist << std::endl;
		}

		if (ray.Hit(sphere->GetBoundingBox(), &dist))
		{
			buffer << L"射线与圆交叉，距离为：" << dist << std::endl;
		}

		if (ray.Hit(cylinder->GetBoundingBox(), &dist))
		{
			buffer << L"射线与圆柱交叉，距离为：" << dist << std::endl;
		}

		if (ray.Hit(cube->GetBoundingBox(), &dist))
		{
			buffer << L"射线与正方体交叉，距离为：" << dist << std::endl;
		}

		if (ray.Hit(mesh->GetBoundingBox(), &dist))
		{
			buffer << L"射线与Mesh交叉，距离为：" << dist << std::endl;
		}

		if ((mesh->GetBoundingBox().Intersects(plane->GetBoundingBox())))
		{
			buffer << L"平面与Mesh交叉" << std::endl;
		}

		if ((mesh->GetBoundingBox().Intersects(cube->GetBoundingBox())))
		{
			buffer << L"cube与Mesh交叉" << std::endl;
		}

		if ((mesh->GetBoundingBox().Intersects(sphere->GetBoundingBox())))
		{
			buffer << L"sphere与Mesh交叉" << std::endl;
		}

		if ((mesh->GetBoundingBox().Intersects(cylinder->GetBoundingBox())))
		{
			buffer << L"cylinder与Mesh交叉" << std::endl;
		}

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

	SceneNode* nodeCube{};
	GameObject* cube{};

	GameObject* planeRtt{};
	SceneNode* nodePlaneRtt{};

	GameObject* plane{};
	SceneNode* nodePlane{};

	SceneNode* nodeCylinder{};
	GameObject* cylinder{};

	SceneNode* nodeSphere{};
	GameObject* sphere{};

	SceneNode* nodeMesh{};
	GameObject* mesh{};

	SceneNode* lightNode{};
	Light* dirLight{};

	SceneNodeCamera* secondCamera{};

	SceneNodeCamera* camera{};
	Core::SVector3 cameraPos = { 0.f, 5.f, -15.f };
	Core::SVector3 cameraRotate = { 0.f, 0.f, 0.f };

	Core::SVector3 sphereRotation = { 0.f, 0.0f, 0.f };

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