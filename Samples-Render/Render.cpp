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

		Launcher* launcher = GetLauncher();
		// SceneManager也是根节点
		sceneMgr = launcher->CreateSceneManager();

		InitLight();

		//json terrainSet;
		//terrainSet["width"] = 1025.0f;
		//terrainSet["depth"] = 1025.0f;
		//terrainSet["m"] = 1025;
		//terrainSet["n"] = 1025;
		//terrainSet["scale"] = 0.003f;
		//terrainSet["heightMap"] = "../Assets/Terrain/heightmap.raw";
		//terrainSet["Shader"] = "Basic";
		//terrain = sceneMgr->CreateGameObject("terrain", terrainSet);
		//terrain->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Terrain/dirt01d.tga"));
		//nodeTerrain = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		//nodeTerrain->SetPosition({ 0.0f, -80.0f, 0.0f });
		//nodeTerrain->AttachObj(terrain);

		// Line
		//json lineSet;
		//lineSet["x1"] = 0;
		//lineSet["y1"] = 0;
		//lineSet["z1"] = 0;
		//lineSet["x2"] = 6;
		//lineSet["y2"] = 6;
		//lineSet["z2"] = 6;
		//lineSet["Shader"] = "Color";
		//line = sceneMgr->CreateGameObject("line", SIMPLE_GAMEOBJECT::SG_LINE3D, lineSet);
		//nodeLine = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		//nodeLine->AttachObj(line);
		//nodeLine->SetPosition({ 0.f, 0.f, 0.f });

		// Point
		//json lineSet;
		//lineSet["x"] = 0;
		//lineSet["y"] = 0;
		//lineSet["z"] = 0;
		//lineSet["Shader"] = "Color";
		//line = sceneMgr->CreateGameObject("point", SIMPLE_GAMEOBJECT::SG_POINT3D, lineSet);
		//nodeLine = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		//nodeLine->AttachObj(line);
		//nodeLine->SetPosition({ 0.f, 0.f, 0.f });

		// Cube
		json cubeSet;
		cubeSet["Shader"] = "Basic";
		cubeSet["width"] = 1;
		cubeSet["height"] = 1;
		cubeSet["depth"] = 1;
		cubeSet["Rasterizer"] = "RT_CULL_NONE";
		cube = sceneMgr->CreateGameObject("cube", SIMPLE_GAMEOBJECT::SG_CUBE, cubeSet);
		cube->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/WireFence.dds"));
		nodeCube = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeCube->AttachObj(cube);
		nodeCube->SetPosition({ 0.f, 1.1f, 3.f });

		// Cylinder
		json cylinderSet;
		cylinderSet["Shader"] = "Basic";
		cylinderSet["bottomRadius"] = 1.0f;
		cylinderSet["topRadius"] = 1.0f;
		cylinderSet["height"] = 3.0f;
		cylinderSet["sliceCount"] = 30;
		cylinderSet["stackCount"] = 60;
		cylinder = sceneMgr->CreateGameObject("cylinder", SIMPLE_GAMEOBJECT::SG_CYLINDER, cylinderSet);
		cylinder->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/stone.dds"));
		nodeCylinder = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeCylinder->AttachObj(cylinder);
		nodeCylinder->SetPosition({ 3.f, 1.1f, 0.f });

		// Mesh
		json meshSet;
		//meshSet["Rasterizer"] = "RT_WIREFRAME";
		mesh = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/mouse/mouse1.obj", meshSet);
		nodeMesh = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh->SetScale({ 5.f, 5.f, 5.f });
		nodeMesh->SetPosition({ -3.f, 10.2f, 0.f });
		nodeMesh->AttachObj(mesh);

		//meshSet["Rasterizer"] = "RT_WIREFRAME";
		mesh2 = sceneMgr->CreateGameObject("mesh2", L"../Assets/Models/box/box.obj", meshSet);
		nodeMesh2 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh2->SetPosition({ 0.f, 0.2f, -3.f });
		nodeMesh2->AttachObj(mesh2);

		// sky
		json sky;
		sky["Shader"] = "SkyBox";
		sky["DepthStencil"] = "DST_LESS_EQUAL";
		sky["Rasterizer"] = "RT_CULL_CLOCKWISE";
		sphere = sceneMgr->CreateGameObject("sphere", SIMPLE_GAMEOBJECT::SG_SPHERE, sky);
		sphere->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/space.dds"));
		nodeSky = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeSky->AttachObj(sphere);
		nodeSky->SetPosition({ 0.f, 0.f, 0.f });

		// nodePlane
		json planeSet;
		planeSet["width"] = 30.0f;
		planeSet["depth"] = 30.0f;
		planeSet["m"] = 5.0f;
		planeSet["n"] = 5.0f;
		planeSet["maxU"] = 5.0f;
		planeSet["maxV"] = 5.0f;
		//planeSet["Rasterizer"] = "RT_WIREFRAME";

		for (size_t i = 0; i < 1; i++)
		{
			plane[i] = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE, planeSet);
			plane[i]->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/checkboard.dds"));
			nodePlane[i] = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
			nodePlane[i]->AttachObj(plane[i]);
		}

		nodePlane[0]->SetPosition({ 0.f, 0.f, 0.f });
		//nodePlane[1]->SetPosition({ 0.f, 15.f, 15.f });
		//nodePlane[2]->SetPosition({ -15.f, 15.f, 0.f });
		//nodePlane[3]->SetPosition({ 0.f, 15.f, -15.f });
		//nodePlane[4]->SetPosition({ 15.f, 15.f, 0.f });
		//nodePlane[1]->SetRotation({ -Core::SM_PIDIV2, 0.f, 0.f });
		//nodePlane[2]->SetRotation({ 0.f, 0.f, -Core::SM_PIDIV2 });
		//nodePlane[3]->SetRotation({ Core::SM_PIDIV2, 0.f, 0.f });
		//nodePlane[4]->SetRotation({ 0.f, 0.f, Core::SM_PIDIV2 });

		particleEmitter = new ParticleEmitter();
		json particleSet;
		particleSet["texture"] = "../Assets/Images/particle_texture.png";
		particleSet["particle_emit_center_x"] = 0.0f;
		particleSet["particle_emit_center_y"] = 0.0f;
		particleSet["particle_emit_center_z"] = 0.0f;
		particleSet["particle_emit_deviation_x"] = 10.f;
		particleSet["particle_emit_deviation_y"] = 0.1f;
		particleSet["particle_emit_deviation_z"] = 10.f;
		particleSet["particle_velocity_x"] = 0.0f;
		particleSet["particle_velocity_y"] = -2.0f;
		particleSet["particle_velocity_z"] = 0.0f;
		particleSet["particle_velocity_variation_angle_x"] = 0;
		particleSet["particle_velocity_variation_angle_y"] = 0;
		particleSet["particle_lifetime_min"] = 3.0f;
		particleSet["particle_lifetime_max"] = 4.0f;
		particleSet["particles_max_num"] = 100.0f;
		particleSet["particle_size"] = 0.2f;
		particleSet["particles_per_second"] = 40;
		particleList = sceneMgr->CreateGameObject("particle", particleEmitter, particleSet);
		nodeParticles = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeParticles->SetPosition({ 0.0f, 3.0f, 0.0f });
		nodeParticles->AttachObj(particleList);

		// nodeWater
		json waterSet;
		waterSet["width"] = 30.0f;
		waterSet["depth"] = 30.0f;
		waterSet["m"] = 5.0f;
		waterSet["n"] = 5.0f;
		waterSet["maxU"] = 5.0f;
		waterSet["maxV"] = 5.0f;
		waterSet["alpha"] = 0.7f;
		waterSet["Blend"] = "BT_TRANSPARENT";
		waterSet["Rasterizer"] = "RT_CULL_NONE";
		waterSet["DepthStencil"] = "DST_NO_DEPTH_WRITE";

		water = sceneMgr->CreateGameObject("water", SIMPLE_GAMEOBJECT::SG_PLANE, waterSet);
		water->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/water.dds"));
		nodeWater = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeWater->AttachObj(water);
		nodeWater->SetPosition({ 0.f, 1.1f, 0.f });

		camera = sceneMgr->AddCameraSceneNode();
		camera->SetIsOrthogonal(false);
		mWindow->AddViewport(camera);

		audio = SoundResourceManager::GetInstance().LoadSound(L"倩女幽魂", L"../Assets/Sounds/邓紫棋+-+喜欢你.wav");
		audio->Play(100, false);

		return true;
	}

	// 每帧开始处理
	bool FrameStarted() override
	{
		particleEmitter->Update(Timer::DeltaTime());

		intersects = mesh->GetBoundingBox().Intersects(mesh2->GetBoundingBox());

		if (Input::DXInput::GetInstance().IsPressed(DIK_ESCAPE))
		{
			return false;
		}
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

		if (Input::DXInput::GetInstance().IsButtonDown(1))
		{
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
		}

		if (cameraChoose == 1)
		{
			ProcessFreeLookCamera();
		}
		else if (cameraChoose == 2)
		{
			ProcessThirdCamera();
		}

		camera->SetPosition(cameraPos);
		nodeSky->SetPosition(cameraPos);
		camera->SetRotation(cameraRotate);

		nodeMesh->SetRotation(rotate);
		nodeMesh->SetPosition(pos);

		return Application::FrameStarted();
	}

	bool FrameUpdated() override
	{
		const Core::SVector2 pos = Input::DXInput::GetInstance().GetMouseLocation();
		Ray ray = Ray::ScreenToRay(*camera, pos);
		if (ray.Hit(cube->GetBoundingBox()))
		{
			RenderSystem2D::GetInstance().DrawTextW(L"选中Cube", { 300, 10 });
		}
		if (ray.Hit(mesh->GetBoundingBox()))
		{
			RenderSystem2D::GetInstance().DrawTextW(L"选中mesh", { 300, 10 });
		}
		if (ray.Hit(mesh2->GetBoundingBox()))
		{
			RenderSystem2D::GetInstance().DrawTextW(L"选中mesh2", { 300, 10 });
		}
		if (ray.Hit(cylinder->GetBoundingBox()))
		{
			RenderSystem2D::GetInstance().DrawTextW(L"选中cylinder", { 300, 10 });
		}

		std::wostringstream buffer;
		buffer << "FPS:" << 1.0f / Timer::DeltaTime() << std::endl
			<< L"CPU占用:" << GetCPUUSe() << std::endl
			<< L"按1切换自由视角摄像机!" << std::endl
			<< L"按2切换第三人称摄像机!" << std::endl
			<< L"CameraPos:(" << cameraPos.x << "," << cameraPos.y << "," << cameraPos.z << ")" << std::endl;
		std::wstring info = buffer.str();

		RenderSystem2D::GetInstance().DrawTextW(info, { 10, 10 });
		if (intersects)
		{
			RenderSystem2D::GetInstance().DrawTextW(L"相交", { 10, 90 });
		}
		else
		{
			RenderSystem2D::GetInstance().DrawTextW(L"未相交", { 10, 90 });
		}
		return Application::FrameUpdated();
	}

	bool FrameEnd() override
	{
		return Application::FrameEnd();
	}

	void InitLight()
	{
		pointLight = sceneMgr->CreateLight("main light", LIGHT_TYPE::LT_POINT);
		pointLight->SetAmbient({ 0.3f, 0.3f, 0.3f, 0.3f });
		pointLight->SetDiffuse({ 1.f, 1.f, 1.f, 1.0f });
		pointLight->SetSpecular({ 0.7f, 0.7f, 0.7f, 1.0f });
		pointLight->SetAtt({ 0.0f, 0.5f, 0.0f });
		pointLight->SetRange(25.0f);
		pointLight->SetPosition({ 0.0f, 3.0f, 0.0f });

		spotLight1 = sceneMgr->CreateLight("spot light1", LIGHT_TYPE::LT_SPOT);
		spotLight1->SetPosition({ 3.0f, 6.0f, 3.0f });
		spotLight1->SetDirection({ 0.0f, -1.0f, 0.0f });
		spotLight1->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
		spotLight1->SetDiffuse({ 1.0f, 0.0f, 0.0f, 1.0f });
		spotLight1->SetSpecular({ 1.0f, 1.0f, 1.0f, 1.0f });
		spotLight1->SetAtt({ 1.0f, 0.0f, 0.0f });
		spotLight1->SetSpot(10.0f);
		spotLight1->SetRange(100.0f);

		spotLight2 = sceneMgr->CreateLight("spot light2", LIGHT_TYPE::LT_SPOT);
		spotLight2->SetPosition({ -3.0f, 6.0f, 3.0f });
		spotLight2->SetDirection({ 0.0f, -1.0f, 0.0f });
		spotLight2->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
		spotLight2->SetDiffuse({ 0.0f, 1.0f, 0.0f, 1.0f });
		spotLight2->SetSpecular({ 1.0f, 1.0f, 1.0f, 1.0f });
		spotLight2->SetAtt({ 1.0f, 0.0f, 0.0f });
		spotLight2->SetSpot(10.0f);
		spotLight2->SetRange(100.0f);

		spotLight3 = sceneMgr->CreateLight("spot light3", LIGHT_TYPE::LT_SPOT);
		spotLight3->SetPosition({ 0.0f, 6.0f, -3.0f });
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

		lightNode = sceneMgr->AddChild(new SceneNodeLight(sceneMgr, sceneMgr));
		lightNode->AttachObj(dirLight);
		//lightNode->AttachObj(pointLight);
		//lightNode->AttachObj(spotLight1);
		//lightNode->AttachObj(spotLight2);
		//lightNode->AttachObj(spotLight3);
	}

	// 第一人称相机
	void ProcessFreeLookCamera()
	{
		const Core::SVector3& forward = camera->GetForward();
		const Core::SVector3& right = camera->GetRight();
		if (Input::DXInput::GetInstance().IsPressed(DIK_UP))
		{
			cameraPos.x += 0.05f * forward.x;
			cameraPos.y += 0.05f * forward.y;
			cameraPos.z += 0.05f * forward.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_DOWN))
		{
			cameraPos.x -= 0.05f * forward.x;
			cameraPos.y -= 0.05f * forward.y;
			cameraPos.z -= 0.05f * forward.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_LEFT))
		{
			cameraPos.x -= 0.05f * right.x;
			cameraPos.y -= 0.05f * right.y;
			cameraPos.z -= 0.05f * right.z;
		}
		if (Input::DXInput::GetInstance().IsPressed(DIK_RIGHT))
		{
			cameraPos.x += 0.05f * right.x;
			cameraPos.y += 0.05f * right.y;
			cameraPos.z += 0.05f * right.z;
		}
	}

	// 第三人称相机
	void ProcessThirdCamera()
	{
		float x = nodeMesh->GetPosition().x + -5 * cosf(cameraRotate.x) * sinf(cameraRotate.y);
		float z = nodeMesh->GetPosition().z + -5 * cosf(cameraRotate.x) * cosf(cameraRotate.y);
		float y = nodeMesh->GetPosition().y + 5 * sinf(cameraRotate.x);
		cameraPos = { x, y, z };
	}

private:
	SceneNode* lightNode{};
	SceneNode* nodeMesh{};
	SceneNode* nodeMesh2{};
	SceneNode* nodeCylinder{};
	SceneNode* nodeCube{};
	SceneNode* nodeSky{};
	SceneNode* nodePlane[5]{};
	SceneNode* nodeParticles{};
	//SceneNode* nodeTerrain{};
	SceneNode* nodeWater{};
	//SceneNode* nodeLine{};

	//GameObject* line{};
	GameObject* water{};
	GameObject* cube{};
	GameObject* sphere{};
	GameObject* cylinder{};
	GameObject* mesh{};
	GameObject* mesh2{};
	GameObject* plane[5]{};
	GameObject* particleList{};
	//GameObject* terrain{};
	IAudioBuffer* audio;
	Light* pointLight{};
	Light* spotLight1{};
	Light* spotLight2{};
	Light* spotLight3{};
	Light* dirLight{};

	SceneNodeCamera* camera{};
	SceneManager* sceneMgr{};
	ParticleEmitter* particleEmitter{};
	Core::SVector3 pos = { -3.f, 0.2f, 0.f };
	Core::SVector3 rotate;
	Core::SVector3 sphereRotate;
	Core::SVector3 cameraPos = { 0.f, 8.6f, -10.f };
	Core::SVector3 cameraRotate = { Core::SM_PIDIV4, 0.f, 0.f };
	int cameraChoose = 1;

	bool intersects;
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
	renderSample.GetLauncher()->Run();
	renderSample.UnInitialize();
	return 0;
}