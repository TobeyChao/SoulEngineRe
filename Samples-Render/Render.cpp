#include "pch.h"
#include "../SoulEngineRe/RenderSystem/DirectX11/D3D11Texture.h"

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
		sceneMgr = launcher->CreateSceneManager();
		mWindow->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Basic");

		shader->SetShadowMatrix(Core::MatrixShadow({ 0.0f, 1.0f, 0.0f, -0.01f }, { 0.0f, 5.0f, -2.0f, 1.0f }));
		shader->SetShadowPlane({ 0.0f, 1.0f, 0.0f, -0.01f });

		//shader->SetFogColor({ 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f });
		//shader->SetFogRange(120.0f);
		//shader->SetFogStart(5.0f);
		//shader->SetFogState(true);

		shader->SetReflectMatrix(Core::MatrixReflect({ 0.0f, 0.0f, 1.0f, -5.0f }));

		InitLight();

		// sky
		json sky;
		sky["Shader"] = "SkyBox";
		sky["DepthStencil"] = "DST_LESS_EQUAL";
		sky["Rasterizer"] = "RT_CULL_CLOCKWISE";
		sphere = sceneMgr->CreateGameObject("sphere", SIMPLE_GAMEOBJECT::SG_SPHERE, sky);
		sphere->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/sky.dds"));
		nodeSky = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeSky->AttachObj(sphere);
		nodeSky->SetPosition({ 0.f, 0.f, 0.f });
		nodeSky->SetScale({ 10.0f, 10.0f, 10.0f });

#pragma region Terrain
		//json terrainSet;
		//terrainSet["width"] = 128.0f;
		//terrainSet["depth"] = 128.0f;
		//terrainSet["m"] = 512;
		//terrainSet["n"] = 512;
		//terrainSet["scale"] = 0.0001f;
		//terrainSet["heightMap"] = "../Assets/Terrain/hm.raw";
		//terrainSet["Shader"] = "Basic";
		//terrain = sceneMgr->CreateGameObject("terrain", terrainSet);
		//terrain->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Terrain/dirt01d.tga"));
		//nodeTerrain = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		//nodeTerrain->SetPosition({ 0.0f, 1.0f, 0.0f });
		//nodeTerrain->AttachObj(terrain);
#pragma endregion

#pragma region Plane
		// nodePlane
		json planeSet2;
		planeSet2["width"] = 10.0f;
		planeSet2["depth"] = 5.0f;
		planeSet2["m"] = 10.0f;
		planeSet2["n"] = 5.0f;
		planeSet2["maxU"] = 1.0f;
		planeSet2["maxV"] = 1.0f;
		planeSet2["Blend"] = "BT_TRANSPARENT";
		planeSet2["DepthStencil"] = "DST_NO_DEPTH_WRITE";
		Material* material = new Material();
		material->ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		material->diffuse = { 1.0f, 1.0f, 1.0f, 0.5f };
		material->specular = { 0.4f, 0.4f, 0.4f, 16.0f };
		plane2 = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE, planeSet2);
		plane2->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/ice.dds"));
		plane2->GetSubMesh(0)->SetMaterial(material);
		nodePlane2 = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodePlane2->AttachObj(plane2);
		nodePlane2->SetPosition({ 0.f, 2.5f, 5.f });
		nodePlane2->SetRotation({ -Core::SM_PIDIV2, 0.f , 0.f });

		// mirror
		json planeMirror;
		planeMirror["width"] = 10.0f;
		planeMirror["depth"] = 5.0f;
		planeMirror["m"] = 10.0f;
		planeMirror["n"] = 5.0f;
		planeMirror["maxU"] = 5.0f;
		planeMirror["maxV"] = 5.0f;
		planeMirror["Blend"] = "BT_NO_COLOR_WRITE";
		planeMirror["DepthStencil"] = "DST_NO_DEPTH_WRITE_WRITE_STECIL";
		planeMirror["StencilRef"] = 1;
		mirror = sceneMgr->CreateGameObject("mirror", SIMPLE_GAMEOBJECT::SG_PLANE, planeMirror);
		nodeMirror = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMirror->AttachObj(mirror);
		nodeMirror->SetPosition({ 0.f, 2.5f, 5.f });
		nodeMirror->SetRotation({ -Core::SM_PIDIV2, 0.f , 0.f });

		// nodePlane
		json planeSet;
		planeSet["width"] = 10.0f;
		planeSet["depth"] = 10.0f;
		planeSet["m"] = 10.0f;
		planeSet["n"] = 10.0f;
		planeSet["maxU"] = 5.0f;
		planeSet["maxV"] = 5.0f;
		planeSet["StencilRef"] = 1;
		planeSet["DepthStencil"] = "DST_WRITE_STECIL";
		plane = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE, planeSet);
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg"));
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg_NORM.jpg"));
		plane->GetSubMesh(0)->EnableReflect(true);
		nodePlane = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodePlane->AttachObj(plane);
		nodePlane->SetPosition({ 0.f, 0.f, 0.f });

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
		//nodeWater->AttachObj(water);
		nodeWater->SetPosition({ 0.f, 1.1f, 0.f });
#pragma endregion

#pragma region SimpleGameObject
		// Cube
		json cubeSet;
		cubeSet["Shader"] = "Basic";
		cubeSet["width"] = 1;
		cubeSet["height"] = 1;
		cubeSet["depth"] = 1;
		cube = sceneMgr->CreateGameObject("cube", SIMPLE_GAMEOBJECT::SG_CUBE, cubeSet);
		cube->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/floor.dds"));
		cube->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/floor_NORM.png"));
		cube->GetSubMesh(0)->EnableShadow(true);
		cube->GetSubMesh(0)->EnableReflect(true);
		nodeCube = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeCube->AttachObj(cube);
		nodeCube->SetPosition({ 0.f, 0.51f, 0.f });

		// Cylinder
		json cylinderSet;
		cylinderSet["Shader"] = "Basic";
		cylinderSet["bottomRadius"] = 1.0f;
		cylinderSet["topRadius"] = 1.0f;
		cylinderSet["height"] = 3.0f;
		cylinderSet["sliceCount"] = 30;
		cylinderSet["stackCount"] = 60;
		cylinder = sceneMgr->CreateGameObject("cylinder", SIMPLE_GAMEOBJECT::SG_CYLINDER, cylinderSet);
		cylinder->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/bricks.dds"));
		cylinder->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/bricks_NORM.png"));
		cylinder->GetSubMesh(0)->EnableShadow(true);
		cylinder->GetSubMesh(0)->EnableReflect(true);
		nodeCylinder = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeCylinder->AttachObj(cylinder);
		nodeCylinder->SetPosition({ 3.f, 1.51f, 0.f });

		// Mesh
		json meshSet;
		mesh = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/box/box.obj", meshSet);
		for (auto it : mesh->GetAllSubMesh())
		{
			it->EnableShadow(true);
			it->EnableReflect(true);
		}
		nodeMesh = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh->SetPosition({ -3.f, 0.f, 0.f });
		nodeMesh->AttachObj(mesh);
#pragma endregion

#pragma region CameraAndRt
		camera = new SceneNodeCamera(sceneMgr, sceneMgr, 101);
		secondCamera = new SceneNodeCamera(sceneMgr, sceneMgr, 102);
		secondCamera->SetPosition({ 0.0f, 10.0f, -10.0f });
		secondCamera->SetRotation({ Core::SM_PIDIV4, 0.f, 0.f });
		sceneMgr->AddChild(camera);
		sceneMgr->AddChild(secondCamera);
		mWindow->AddViewport(camera, 0, 0.0f, 0.0f, 1.f, 1.f);
		ITexture* tex = TextureManager::GetInstance().CreateTexture(L"rtt", { 1024, 1024 }, false);
		D3D11RenderTexture* rtt = dynamic_cast<D3D11RenderTexture*>(tex);
		rtt->AddViewport(secondCamera, 1, 0.0f, 0.0f, 1.f, 1.f);
		rtt->SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		GetLauncher()->GetActiveRenderSystem()->AddRenderTarget(rtt);
#pragma endregion
		return true;
	}

	// 每帧开始处理
	bool FrameStarted() override
	{
		//particleEmitter->Update(Timer::DeltaTime());
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

		camera->SetPosition(cameraPos);
		nodeSky->SetPosition(cameraPos);
		camera->SetRotation(cameraRotate);

		nodeMesh->SetRotation(rotate);
		nodeMesh->SetPosition(pos);

		return Application::FrameStarted();
	}

	bool FrameUpdated() override
	{
		//const Core::SVector2 pos = Input::DXInput::GetInstance().GetMouseLocation();
		//Ray ray = Ray::ScreenToRay(*camera, pos);
		//if (ray.Hit(cube->GetBoundingBox()))
		//{
		//	RenderSystem2D::GetInstance().DrawTextW(L"选中Cube", { 300, 10 });
		//}
		//if (ray.Hit(mesh->GetBoundingBox()))
		//{
		//	RenderSystem2D::GetInstance().DrawTextW(L"选中mesh", { 300, 10 });
		//}
		//if (ray.Hit(mesh2->GetBoundingBox()))
		//{
		//	RenderSystem2D::GetInstance().DrawTextW(L"选中mesh2", { 300, 10 });
		//}
		//if (ray.Hit(cylinder->GetBoundingBox()))
		//{
		//	RenderSystem2D::GetInstance().DrawTextW(L"选中cylinder", { 300, 10 });
		//}

		std::wostringstream buffer;
		buffer << "FPS:" << 1.0f / Timer::DeltaTime() << std::endl
			<< L"CPU占用:" << GetCPUUSe() << std::endl
			<< L"按1切换自由视角摄像机!" << std::endl
			<< L"按2切换第三人称摄像机!" << std::endl
			<< L"CameraPos:(" << cameraPos.x << "," << cameraPos.y << "," << cameraPos.z << ")" << std::endl
			/*<< L"RayDir:(" << ray.mDirection.x << "," << ray.mDirection.y << "," << ray.mDirection.z << ")" << std::endl*/;
		std::wstring info = buffer.str();

		RenderSystem2D::GetInstance().DrawTextW(info, { 10, 10 });
		return Application::FrameUpdated();
	}

	bool FrameEnd() override
	{
		return Application::FrameEnd();
	}

	void InitLight()
	{
		pointLight = sceneMgr->CreateLight("main light", LIGHT_TYPE::LT_POINT);
		pointLight->SetAmbient({ 0.8f, 0.8f, 0.8f, 1.0f });
		pointLight->SetDiffuse({ 1.f, 1.f, 1.f, 1.0f });
		pointLight->SetSpecular({ 0.7f, 0.7f, 0.7f, 1.0f });
		pointLight->SetAtt({ 0.0f, 0.5f, 0.0f });
		pointLight->SetRange(45.0f);
		pointLight->SetPosition({ 0.0f, 5.0f, -2.0f });

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
		dirLight->SetAmbient({ 1.f, 1.f, 1.f, 1.0f });
		dirLight->SetDiffuse({ 1.f, 1.f, 1.f, 1.0f });
		dirLight->SetSpecular({ 0.1f, 0.1f, 0.1f, 1.0f });
		dirLight->SetDirection({ -0.577f, -0.577f, 0.577f });

		lightNode = sceneMgr->AddChild(new SceneNodeLight(sceneMgr, sceneMgr));
		//lightNode->AttachObj(dirLight);
		lightNode->AttachObj(pointLight);
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
		float x = nodeCube->GetPosition().x + -5 * cosf(cameraRotate.x) * sinf(cameraRotate.y);
		float z = nodeCube->GetPosition().z + -5 * cosf(cameraRotate.x) * cosf(cameraRotate.y);
		float y = nodeCube->GetPosition().y + 5 * sinf(cameraRotate.x);
		cameraPos = { x, y, z };
	}

private:
	SceneNode* lightNode{};
	SceneNode* nodeMesh{};
	SceneNode* nodeCylinder{};
	SceneNode* nodeCube{};
	SceneNode* nodeSky{};
	SceneNode* nodeMirror{};
	SceneNode* nodePlane{};
	SceneNode* nodePlane2{};
	SceneNode* nodeParticles{};
	SceneNode* nodeTerrain{};
	SceneNode* nodeWater{};

	GameObject* mirror{};
	GameObject* water{};
	GameObject* cube{};
	GameObject* sphere{};
	GameObject* cylinder{};
	GameObject* mesh{};
	GameObject* plane{};
	GameObject* plane2{};
	GameObject* particleList{};
	GameObject* terrain{};
	IAudioBuffer* audio;
	Light* pointLight{};
	Light* spotLight1{};
	Light* spotLight2{};
	Light* spotLight3{};
	Light* dirLight{};

	SceneNodeCamera* camera{};
	SceneNodeCamera* secondCamera{};
	SceneManager* sceneMgr{};
	ParticleEmitter* particleEmitter{};
	Core::SVector3 pos = { -3.f, 0.2f, 0.f };
	Core::SVector3 rotate;
	Core::SVector3 sphereRotate;
	Core::SVector3 cameraPos = { 0.f, 8.f, -8.f };
	Core::SVector3 cameraRotate = { Core::SM_PIDIV4, 0.f, 0.f };
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
	renderSample.GetLauncher()->Run();
	renderSample.UnInitialize();
	return 0;
}