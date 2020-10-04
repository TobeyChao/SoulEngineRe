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
		mWindow->SetClearColor({ 0.752941251f, 0.752941251f, 0.752941251f, 1.0f });

		Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Basic");

		shader->SetFogColor({ 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f });
		shader->SetFogRange(200.0f);
		shader->SetFogStart(80.0f);
		shader->SetFogState(true);

		shader->SetShadowMatrix(Core::MatrixShadow({ 0.0f, 1.0f, 0.0f, -0.01f }, { 0.5f, 1.f, -0.5f, 0.0f }));
		shader->SetShadowPlane({ 0.0f, 1.0f, 0.0f, -0.01f });

		shader->SetReflectMatrix(Core::MatrixReflect({ 0.0f, 0.0f, 1.0f, -5.0f }));

		InitLight();

		// sky
		json skySet;
		skySet["Shader"] = "SkyBox";
		sky = sceneMgr->CreateGameObject("sky", SIMPLE_GAMEOBJECT::SG_SPHERE, skySet);
		sky->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/desertcube1024.dds"));
		sky->GetSubMesh(0)->SetDepthStencil(DepthStencilType::DST_LESS_EQUAL, 0);
		sky->GetSubMesh(0)->SetRasterizer(RasterizerType::RT_CULL_CLOCKWISE);
		nodeSky = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeSky->SetPosition({ 0.f, 0.f, 0.f });
		nodeSky->SetScale({ 10.0f, 10.0f, 10.0f });

#pragma region Terrain
		json terrainSet;
		terrainSet["width"] = 1024.0f;
		terrainSet["depth"] = 1024.0f;
		terrainSet["m"] = 512;
		terrainSet["n"] = 512;
		terrainSet["scale"] = 0.0005f;
		terrainSet["heightMap"] = "../Assets/Terrain/hm.raw";
		terrainSet["Shader"] = "Basic";
		terrain = sceneMgr->CreateGameObject("terrain", terrainSet);
		terrain->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Terrain/dirt01d.tga"));
		nodeTerrain = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeTerrain->SetPosition({ 0.0f, -33.0f, 0.0f });
		nodeTerrain->AttachObj(terrain);
#pragma endregion

#pragma region Plane
		// nodePlane
		json planeSet;
		planeSet["width"] = 20.0f;
		planeSet["depth"] = 20.0f;
		planeSet["m"] = 10.0f;
		planeSet["n"] = 10.0f;
		planeSet["maxU"] = 5.0f;
		planeSet["maxV"] = 5.0f;
		plane = sceneMgr->CreateGameObject("plane", SIMPLE_GAMEOBJECT::SG_PLANE, planeSet);
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/bricks.dds"));
		plane->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/bricks_NORM.png"));
		plane->GetSubMesh(0)->SetDepthStencil(DepthStencilType::DST_WRITE_STECIL, 2);
		nodePlane = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodePlane->AttachObj(plane);
		nodePlane->SetPosition({ 0.f, 0.f, 0.f });

		// nodeWater
		json waterSet;
		waterSet["width"] = 20.0f;
		waterSet["depth"] = 20.0f;
		waterSet["m"] = 5.0f;
		waterSet["n"] = 5.0f;
		waterSet["maxU"] = 5.0f;
		waterSet["maxV"] = 5.0f;
		waterSet["alpha"] = 0.5f;
		water = sceneMgr->CreateGameObject("water", SIMPLE_GAMEOBJECT::SG_PLANE, waterSet);
		water->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/water.dds"));
		water->GetSubMesh(0)->SetBlend(BlendType::BT_TRANSPARENT);
		water->GetSubMesh(0)->SetDepthStencil(DepthStencilType::DST_NO_DEPTH_WRITE, 0);
		water->GetSubMesh(0)->SetRasterizer(RasterizerType::RT_CULL_NONE);
		nodeWater = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		//nodeWater->AttachObj(water);
		nodeWater->SetPosition({ 0.f, 1.5f, 0.f });
#pragma endregion

#pragma region SimpleGameObject
		// Cylinder
		json cylinderSet;
		cylinderSet["Shader"] = "Basic";
		cylinderSet["bottomRadius"] = 1.0f;
		cylinderSet["topRadius"] = 1.0f;
		cylinderSet["height"] = 5.0f;
		cylinderSet["sliceCount"] = 20;
		cylinderSet["stackCount"] = 10;
		for (size_t i = 0; i < 4; i++)
		{
			cylinder[i] = sceneMgr->CreateGameObject("cylinder", SIMPLE_GAMEOBJECT::SG_CYLINDER, cylinderSet);
			cylinder[i]->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg"));
			cylinder[i]->GetSubMesh(0)->EnableShadow(true);
			nodeCylinder[i] = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
			nodeCylinder[i]->AttachObj(cylinder[i]);
		}

		cylinder[0]->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg_NORM.jpg"));
		cylinder[3]->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/catton_grpund.jpg_NORM.jpg"));
		nodeCylinder[0]->SetPosition({ 8.f, 2.51f, 8.f });
		nodeCylinder[1]->SetPosition({ 8.f, 2.51f, -8.f });
		nodeCylinder[2]->SetPosition({ -8.f, 2.51f, 8.f });
		nodeCylinder[3]->SetPosition({ -8.f, 2.51f, -8.f });

		// Cylinder
		json sphereSet;
		for (size_t i = 0; i < 4; i++)
		{
			sphere[i] = sceneMgr->CreateGameObject("sphere", SIMPLE_GAMEOBJECT::SG_SPHERE, sphereSet);
			sphere[i]->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/Redocn_2013122305390429.jpg"));
			sphere[i]->GetSubMesh(0)->PushTexture(TextureManager::GetInstance().GetTexture(L"../Assets/Images/Redocn_2013122305390429_NORM.png"));
			sphere[i]->GetSubMesh(0)->EnableShadow(true);
			nodeSphere[i] = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
			nodeSphere[i]->AttachObj(sphere[i]);
		}
		nodeSphere[0]->SetPosition({ 8.f, 6.51f, 8.f });
		nodeSphere[1]->SetPosition({ 8.f, 6.51f, -8.f });
		nodeSphere[2]->SetPosition({ -8.f, 6.51f, 8.f });
		nodeSphere[3]->SetPosition({ -8.f, 6.51f, -8.f });

		// Mesh
		mesh = sceneMgr->CreateGameObject("mesh", L"../Assets/Models/house2/house2.obj", json());
		for (auto it : mesh->GetAllSubMesh())
		{
			it->EnableShadow(true);
		}
		nodeMesh = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeMesh->SetPosition({ 0.f, 0.5f, 0.f });
		nodeMesh->SetScale({ 0.5f, 0.5f, 0.5f });
		nodeMesh->AttachObj(mesh);
#pragma endregion

		// Particles
		particleEmitter = new ParticleEmitter();
		json particleSet;
		particleSet["texture"] = "../Assets/Images/particle_texture.png";
		particleSet["particle_emit_deviation_x"] = 10.f;
		particleSet["particle_emit_deviation_y"] = 0.1f;
		particleSet["particle_emit_deviation_z"] = 10.f;
		particleSet["particle_velocity_x"] = 0.0f;
		particleSet["particle_velocity_y"] = -3.0f;
		particleSet["particle_velocity_z"] = 0.0f;
		particleSet["particle_velocity_variation_angle_x"] = 0;
		particleSet["particle_velocity_variation_angle_y"] = 0;
		particleSet["particle_lifetime_min"] = 3.0f;
		particleSet["particle_lifetime_max"] = 4.0f;
		particleSet["particles_max_num"] = 100.0f;
		particleSet["particle_size"] = 0.2f;
		particleSet["particles_per_second"] = 20;
		particleList = sceneMgr->CreateGameObject("particle", particleEmitter, particleSet);
		nodeParticles = sceneMgr->AddChild(new SceneNodeRenderable(sceneMgr, sceneMgr));
		nodeParticles->SetPosition({ 0.0f, 10.0f, 0.0f });
		nodeParticles->AttachObj(particleList);

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
		particleEmitter->Update(Timer::DeltaTime());
		if (Input::DXInput::GetInstance().IsPressed(DIK_ESCAPE))
		{
			return false;
		}
		sphereRotation.y += Timer::DeltaTime();

		if (Input::DXInput::GetInstance().IsPressed(DIK_1))
		{
			cameraChoose = 1;
		}
		else if (Input::DXInput::GetInstance().IsPressed(DIK_2))
		{
			cameraChoose = 2;
		}

		if (Input::DXInput::GetInstance().IsPressed(DIK_F1) && !Input::DXInput::GetInstance().IsLastPressed(DIK_F1))
		{
			if (fog == 2)
			{
				nodeSky->DetachObj(sky);
				Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Basic");
				shader->SetFogState(true);
				fog = 1;
			}
		}
		else if (Input::DXInput::GetInstance().IsPressed(DIK_F2) && !Input::DXInput::GetInstance().IsLastPressed(DIK_F2))
		{
			if (fog == 1)
			{
				Shader* shader = ShaderManager::GetInstance().GetShaderByName(L"Basic");
				shader->SetFogState(false);
				nodeSky->AttachObj(sky);
				fog = 2;
			}
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

		for (size_t i = 0; i < 4; i++)
		{
			nodeSphere[i]->SetRotation(sphereRotation);
		}
		return Application::FrameStarted();
	}

	bool FrameUpdated() override
	{
		std::wostringstream buffer;
		buffer << "FPS:" << 1.0f / Timer::DeltaTime() << std::endl
			<< L"CPU占用:" << GetCPUUSe() << std::endl
			<< L"按1切换自由视角摄像机!" << std::endl
			<< L"按2切换第三人称摄像机!" << std::endl
			<< L"按F1切换雾效" << std::endl
			<< L"按F2切换天空盒" << std::endl
			<< L"CameraPos:(" << cameraPos.x << "," << cameraPos.y << "," << cameraPos.z << ")" << std::endl;
		std::wstring info = buffer.str();

		//RenderSystem2D::GetInstance().DrawTextW(info, { 10, 10 });
		return Application::FrameUpdated();
	}

	bool FrameEnd() override
	{
		return Application::FrameEnd();
	}

	void InitLight()
	{
		pointLight = sceneMgr->CreateLight("main light", LIGHT_TYPE::LT_POINT);
		pointLight->SetAmbient({ 0.3f, 0.3f, 0.3f, 1.0f });
		pointLight->SetDiffuse({ 0.6f, 0.6f, 0.6f, 1.0f });
		pointLight->SetSpecular({ 0.2f, 0.2f, 0.2f, 1.0f });
		pointLight->SetAtt({ 0.0f, 0.1f, 0.0f });
		pointLight->SetRange(25.0f);
		pointLight->SetPosition({ 0.0f, 10.0f, 0.0f });

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
		dirLight->SetSpecular({ 0.5f, 0.5f, 0.5f, 16.0f });
		dirLight->SetDirection({ -0.577f, -0.577f, 0.577f });

		lightNode = sceneMgr->AddChild(new SceneNodeLight(sceneMgr, sceneMgr));
		lightNode->AttachObj(dirLight);
		//lightNode->AttachObj(pointLight);
		lightNode->AttachObj(spotLight1);
		lightNode->AttachObj(spotLight2);
		lightNode->AttachObj(spotLight3);
	}

	// 第一人称相机
	void ProcessFreeLookCamera()
	{
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
	}

	// 第三人称相机
	void ProcessThirdCamera()
	{
		float x = nodeMesh->GetPosition().x + -10 * cosf(cameraRotate.x) * sinf(cameraRotate.y);
		float z = nodeMesh->GetPosition().z + -10 * cosf(cameraRotate.x) * cosf(cameraRotate.y);
		float y = nodeMesh->GetPosition().y + 10 * sinf(cameraRotate.x);
		cameraPos = { x, y, z };
	}

private:
	SceneNode* nodeSky{};
	GameObject* sky{};


	SceneNode* nodeMesh{};
	GameObject* mesh{};

	SceneNode* nodeCylinder[4]{};
	GameObject* cylinder[4]{};

	SceneNode* nodeSphere[4]{};
	GameObject* sphere[4]{};

	SceneNode* nodeParticles{};
	GameObject* particleList{};
	ParticleEmitter* particleEmitter{};

	SceneNode* nodeTerrain{};
	GameObject* terrain{};

	SceneNode* nodeWater{};
	GameObject* water{};

	GameObject* plane{};
	SceneNode* nodePlane{};

	GameObject* plane2{};
	SceneNode* nodePlane2{};

	IAudioBuffer* audio;

	SceneNode* lightNode{};
	Light* pointLight{};
	Light* spotLight1{};
	Light* spotLight2{};
	Light* spotLight3{};
	Light* dirLight{};

	SceneNodeCamera* camera{};
	SceneManager* sceneMgr{};
	Core::SVector3 sphereRotation = { 0.f, 0.51f, 0.f };
	Core::SVector3 cameraPos = { 0.f, 8.f, -8.f };
	Core::SVector3 cameraRotate = { Core::SM_PIDIV4, 0.f, 0.f };
	int cameraChoose = 1;
	int fog = 1;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	AudioSample renderSample;
	if (!renderSample.Initialize())
	{
		return 1;
	}
	renderSample.GetLauncher()->Run();
	renderSample.UnInitialize();
	return 0;
}