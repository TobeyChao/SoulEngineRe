#include "SceneManager.h"
#include "SceneNodeCamera.h"
#include "GameObject.h"

#include "SubMesh.h"
#include "ParticalList.h"
#include "ParticleEmitter.h"
#include "TerrainCreator.h"

#include "Light.h"

#include "../Launcher.h"
#include "../RenderSystem/RenderSystem.h"
#include "../Resource/ObjLoader.h"
#include "GeometryGenerator.h"
#include "../Resource/TextureManager.h"

namespace Soul
{
	SceneManager::SceneManager()
		:
		SceneNode(nullptr, nullptr),
		mActiveCamera(nullptr),
		mRenderSystem(nullptr),
		mActiveViewport(nullptr)
	{
		mSceneManager = this;
		const auto admin = Launcher::GetInstancePtr();
		if (admin)
		{
			mRenderSystem = Launcher::GetInstance().GetActiveRenderSystem();
		}
	}

	SceneManager::~SceneManager()
	{
		mCameraNodeList.clear();
		mLightNodeList.clear();
		mRenderableNodeList.clear();
	}

	SceneNodeCamera* SceneManager::AddCameraSceneNode(SceneNode* parent,
		const Core::SVector3& position, const Core::SVector3& lookAt, size_t id, bool makeActive)
	{
		if (!parent)
			parent = this;

		const auto node = new SceneNodeCamera(parent, this, id, position, lookAt);

		if (makeActive)
			SetActiveCamera(node);

		return node;
	}

	SceneNode* SceneManager::AddSkyBoxSceneNode(ITexture* texture, SceneNode* parent, size_t id)
	{
		return nullptr;
	}

	SceneNode* SceneManager::GetRootSceneNode()
	{
		return this;
	}

	SceneNode* SceneManager::GetSceneNodeFromId(int id, SceneNode* start)
	{
		return nullptr;
	}

	SceneNode* SceneManager::GetSceneNodeFromName(const std::wstring& name, SceneNode* start)
	{
		return nullptr;
	}

	SceneNodeCamera* SceneManager::GetActiveCamera() const
	{
		return mActiveCamera;
	}

	void SceneManager::SetActiveCamera(SceneNodeCamera* camera)
	{
		mActiveCamera = camera;
	}

	unsigned int SceneManager::RegisterNode(SceneNode* node, E_SCENENODE_TYPES pass)
	{
		unsigned int taken = 0u;
		switch (pass)
		{
		case E_SCENENODE_TYPES::EST_CAMERA:
			taken = 1;
			for (size_t i = 0; i != mCameraNodeList.size(); ++i)
			{
				if (mCameraNodeList[i] == node)
				{
					taken = 0;
					break;
				}
			}
			if (taken)
			{
				mCameraNodeList.push_back(node);
			}
			break;
		case E_SCENENODE_TYPES::EST_SOLID:
			mRenderableNodeList.push_back(node);
			taken = 1;
			break;
		case E_SCENENODE_TYPES::EST_LIGHT:
			mLightNodeList.push_back(node);
			taken = 1;
			break;
		default:
			break;
		}
		return taken;
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name, ParticleEmitter* particleEmmiter, const json& createParameters)
	{
		std::wstring textureNameW;
		if (createParameters.contains("texture"))
		{
			std::string textureName = createParameters["texture"];
			textureNameW = StringToWstring(textureName);
		}
		else
		{
			return nullptr;
		}
		if (particleEmmiter)
		{
			GameObject* newGameObject = new GameObject(name);
			SubMesh* subMesh = new ParticalList(name);
			subMesh->SetBlend(BlendType::BT_TRANSPARENT);
			subMesh->SetDepthStencil(DepthStencilType::DST_NO_DEPTH_WRITE, 0);
			subMesh->SetRasterizer(RasterizerType::RT_CULL_NONE);
			subMesh->SetShader(ShaderManager::GetInstance().GetShaderByName(L"Particle"));
			subMesh->PushTexture(TextureManager::GetInstance().GetTexture(textureNameW));
			particleEmmiter->Initialize(subMesh, createParameters);
			newGameObject->PushSubMesh(subMesh);
			return newGameObject;
		}
		return nullptr;
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name, const json& createParameters)
	{
		SubMesh* subMesh = new SubMesh(name);
		// Material
		Material* material = new Material();
		material->ambient = { 0.7250f, 0.7100f, 0.6800f, 1.0f };
		material->diffuse = { 0.7250f, 0.7100f, 0.6800f, 1.0f };
		material->specular = { 0.5f, 0.5f, 0.5f, 5.0f };
		subMesh->SetMaterial(material);

		SetCustomEffect(subMesh, createParameters);

		TerrainCreator::CreateTerrainWithHeightMap(createParameters, subMesh);
		subMesh->InitializeBuffer();
		GameObject* newGameObject = new GameObject(name);
		newGameObject->PushSubMesh(subMesh);
		return newGameObject;
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name, SIMPLE_GAMEOBJECT simpleGameObject, const json& createParameters)
	{
		GameObject* newGameObject = nullptr;

		SubMesh* subMesh = new SubMesh(name);

		// Material
		Material* material = new Material();
		material->ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
		material->diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
		if (createParameters.contains("alpha"))
		{
			material->diffuse.w = createParameters["alpha"];
		}
		material->specular = { 0.2f, 0.2f, 0.2f, 16.0f };
		subMesh->SetMaterial(material);

		SetCustomEffect(subMesh, createParameters);

		// Mesh
		GeometryGenerator geoGen;
		switch (simpleGameObject)
		{
		case SIMPLE_GAMEOBJECT::SG_CUBE:
		{
			newGameObject = new GameObject(name);
			newGameObject->PushSubMesh(subMesh);
			float width = 1.f;
			float height = 1.f;
			float depth = 1.f;
			if (createParameters.contains("width"))
			{
				width = createParameters["width"];
			}
			if (createParameters.contains("height"))
			{
				height = createParameters["height"];
			}
			if (createParameters.contains("depth"))
			{
				depth = createParameters["depth"];
			}
			geoGen.CreateBox(width, height, depth, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
		}break;
		case SIMPLE_GAMEOBJECT::SG_PLANE:
		{
			newGameObject = new GameObject(name);
			newGameObject->PushSubMesh(subMesh);
			float width = 1.f;
			float depth = 1.f;
			unsigned m = 2;
			unsigned n = 2;
			float maxU = 1.0f;
			float maxV = 1.0f;
			if (createParameters.contains("width"))
			{
				width = createParameters["width"];
			}
			if (createParameters.contains("depth"))
			{
				depth = createParameters["depth"];
			}
			if (createParameters.contains("m"))
			{
				m = createParameters["m"];
			}
			if (createParameters.contains("n"))
			{
				n = createParameters["n"];
			}
			if (createParameters.contains("maxU"))
			{
				maxU = createParameters["maxU"];
			}
			if (createParameters.contains("maxV"))
			{
				maxV = createParameters["maxV"];
			}

			geoGen.CreateGrid(width, depth, m, n, { maxU, maxV }, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
			break;
		}
		case SIMPLE_GAMEOBJECT::SG_SPHERE:
		{
			newGameObject = new GameObject(name);
			newGameObject->PushSubMesh(subMesh);
			float radius = 1.0f;
			unsigned sliceCount = 20;
			unsigned stackCount = 20;
			if (createParameters.contains("radius"))
			{
				radius = createParameters["radius"];
			}
			if (createParameters.contains("sliceCount"))
			{
				sliceCount = createParameters["sliceCount"];
			}
			if (createParameters.contains("stackCount"))
			{
				stackCount = createParameters["stackCount"];
			}
			geoGen.CreateSphere(radius, sliceCount, stackCount, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
			break;
		}
		case SIMPLE_GAMEOBJECT::SG_CYLINDER:
		{
			newGameObject = new GameObject(name);
			newGameObject->PushSubMesh(subMesh);
			float bottomRadius = 1.0f;
			float topRadius = 1.0f;
			float height = 3.0f;
			unsigned sliceCount = 30;
			unsigned stackCount = 60;
			if (createParameters.contains("bottomRadius"))
			{
				bottomRadius = createParameters["bottomRadius"];
			}
			if (createParameters.contains("topRadius"))
			{
				topRadius = createParameters["topRadius"];
			}
			if (createParameters.contains("height"))
			{
				height = createParameters["height"];
			}
			if (createParameters.contains("sliceCount"))
			{
				sliceCount = createParameters["sliceCount"];
			}
			if (createParameters.contains("stackCount"))
			{
				stackCount = createParameters["stackCount"];
			}
			geoGen.CreateCylinder(bottomRadius, topRadius, height, sliceCount, stackCount, *subMesh->GetOriginalMeshDataPtr());
			subMesh->InitializeBuffer();
			break;
		}
		case SIMPLE_GAMEOBJECT::SG_LINE3D:
		{
			float x1 = 0.f, y1 = 0.f, z1 = 0.f;
			float x2 = 1.f, y2 = 1.f, z2 = 1.f;
			if (createParameters.contains("x1"))
			{
				x1 = createParameters["x1"];
			}
			if (createParameters.contains("y1"))
			{
				y1 = createParameters["y1"];
			}
			if (createParameters.contains("z1"))
			{
				z1 = createParameters["z1"];
			}
			if (createParameters.contains("x2"))
			{
				x2 = createParameters["x2"];
			}
			if (createParameters.contains("y2"))
			{
				y2 = createParameters["y2"];
			}
			if (createParameters.contains("z2"))
			{
				z2 = createParameters["z2"];
			}
			newGameObject = new GameObject(name);
			newGameObject->PushSubMesh(subMesh);
			geoGen.CreateLine3D({ x1, y1, z1 }, { x2, y2, z2 }, *subMesh->GetOriginalMeshDataPtr());
			subMesh->GetRenderParameter().mPrimitiveTopology = PrimitiveTopology::PT_LINELIST;
			subMesh->InitializeBuffer();
			break;
		}
		case SIMPLE_GAMEOBJECT::SG_POINT3D:
		{
			float x = 0.f, y = 0.f, z = 0.f;
			if (createParameters.contains("x"))
			{
				x = createParameters["x"];
			}
			if (createParameters.contains("y"))
			{
				y = createParameters["y"];
			}
			if (createParameters.contains("z"))
			{
				z = createParameters["z"];
			}
			newGameObject = new GameObject(name);
			newGameObject->PushSubMesh(subMesh);
			geoGen.CreatePoint3D({ x, y, z }, *subMesh->GetOriginalMeshDataPtr());
			subMesh->GetRenderParameter().mPrimitiveTopology = PrimitiveTopology::PT_POINTLIST;
			subMesh->InitializeBuffer();
			break;
		}
		}
		newGameObject->BuildBoundingBox();
		return newGameObject;
	}

	GameObject* SceneManager::CreateGameObject(const std::string& name, const std::wstring& meshFilePath, const json& createParameters)
	{
		ObjLoader loader;
		loader.LoadObjModel(meshFilePath);
		auto& subMeshes = loader.GetSubMeshes();
		if (subMeshes.empty())
		{
			loader.Clean();
			return nullptr;
		}
		GameObject* gameObject = new GameObject(name);
		for (auto it : subMeshes)
		{
			SetCustomEffect(it, createParameters);
			it->InitializeBuffer();
			gameObject->PushSubMesh(it);
		}
		gameObject->BuildBoundingBox();
		loader.Clean();
		return gameObject;
	}

	Light* SceneManager::CreateLight(const std::string& name, LIGHT_TYPE lightType)
	{
		Light* light = nullptr;
		switch (lightType)
		{
		case LIGHT_TYPE::LT_DIRECTIONAL:
			light = new Light(name, lightType);
			break;
		case LIGHT_TYPE::LT_POINT:
			light = new Light(name, lightType);
			break;
		case LIGHT_TYPE::LT_SPOT:
			light = new Light(name, lightType);
			break;
		default:
			break;
		}
		return light;
	}

	void SceneManager::SetViewport(Viewport* viewport)
	{
		mActiveViewport = viewport;
		mRenderSystem->SetViewport(viewport);
	}

	void SceneManager::Render()
	{
	}

	void SceneManager::SetCustomEffect(SubMesh* subMesh, const json& effctSetting)
	{
		// DepthStencil
		if (effctSetting.contains("DepthStencil"))
		{
			std::string depthStencil = effctSetting["DepthStencil"];
			if (depthStencil == "DST_LESS_EQUAL")
			{
				subMesh->SetDepthStencil(DepthStencilType::DST_LESS_EQUAL, 0);
			}
			else if (depthStencil == "DST_NO_DEPTH_WRITE")
			{
				subMesh->SetDepthStencil(DepthStencilType::DST_NO_DEPTH_WRITE, 0);
			}
			else if (depthStencil == "DST_NO_DEPTH_WRITE_WRITE_STECIL")
			{
				UINT StencilRef = 0;
				if (effctSetting.contains("StencilRef"))
				{
					StencilRef = effctSetting["StencilRef"];
				}
				subMesh->SetDepthStencil(DepthStencilType::DST_NO_DEPTH_WRITE_WRITE_STECIL, StencilRef);
			}
			else if (depthStencil == "DST_WRITE_STECIL")
			{
				UINT StencilRef = 0;
				if (effctSetting.contains("StencilRef"))
				{
					StencilRef = effctSetting["StencilRef"];
				}
				subMesh->SetDepthStencil(DepthStencilType::DST_WRITE_STECIL, StencilRef);
			}
			else if (depthStencil == "DST_DRAW_WITH_STECIL")
			{
				UINT StencilRef = 0;
				if (effctSetting.contains("StencilRef"))
				{
					StencilRef = effctSetting["StencilRef"];
				}
				subMesh->SetDepthStencil(DepthStencilType::DST_DRAW_WITH_STECIL, StencilRef);
				
			}
			else if (depthStencil == "DST_NO_DOUBLE_BLEND")
			{
				UINT StencilRef = 0;
				if (effctSetting.contains("StencilRef"))
				{
					StencilRef = effctSetting["StencilRef"];
				}
				subMesh->SetDepthStencil(DepthStencilType::DST_NO_DOUBLE_BLEND, StencilRef);
			}
		}

		// Rasterizer
		if (effctSetting.contains("Rasterizer"))
		{
			std::string rasterizer = effctSetting["Rasterizer"];
			if (rasterizer == "RT_CULL_CLOCKWISE")
			{
				subMesh->SetRasterizer(RasterizerType::RT_CULL_CLOCKWISE);
			}
			else if (rasterizer == "RT_CULL_NONE")
			{
				subMesh->SetRasterizer(RasterizerType::RT_CULL_NONE);
			}
			else if (rasterizer == "RT_CULL_COUNTERCLOCKWISE")
			{
				subMesh->SetRasterizer(RasterizerType::RT_CULL_COUNTERCLOCKWISE);
			}
			else if (rasterizer == "RT_WIREFRAME")
			{
				subMesh->SetRasterizer(RasterizerType::RT_WIREFRAME);
			}
		}

		// Blend
		if (effctSetting.contains("Blend"))
		{
			std::string blend = effctSetting["Blend"];
			if (blend == "BT_TRANSPARENT")
			{
				subMesh->SetBlend(BlendType::BT_TRANSPARENT);
			}
		}

		// Shader
		Shader* shader = nullptr;
		if (effctSetting.contains("Shader"))
		{
			const std::string shaderName = effctSetting["Shader"];
			shader = ShaderManager::GetInstance().GetShaderByName(StringToWstring(shaderName));
		}
		else
		{
			shader = ShaderManager::GetInstance().GetShaderByName(L"Basic");
		}
		subMesh->SetShader(shader);
	}

	void SceneManager::DrawAll(SceneNodeCamera* camera, Viewport* viewport)
	{
		// 设置激活摄像机
		SetActiveCamera(camera);
		// 设置视口
		SetViewport(viewport);
		// 注册所有可见场景节点
		OnRegisterSceneNode();
		// 清理缓冲区
		mRenderSystem->Clear({ 0.14f, 0.14f, 0.152f, 1.0f });
		// 遍历所有灯光节点
		for (auto& i : mLightNodeList)
		{
			i->Render();
		}
		// 遍历所有渲染节点
		for (auto& i : mRenderableNodeList)
		{
			i->Render();
		}
		// 对每个节点所绑定的SubMesh进行渲染
		while (!mAllAttachedSubMesh.empty())
		{
			SubMesh* sm = mAllAttachedSubMesh.front();
			const BoundingBox& bb = sm->GetParent()->GetBoundingBox();
			// 到三边距离均为0不进行裁剪
			if (bb.mLengthToSides != Core::SVector3{ 0.0f, 0.0f, 0.0f})
			{
				if (!mActiveCamera->CheckRectangle(bb))
				{
					std::cout << sm->GetParent()->GetName() << std::endl;
					mAllAttachedSubMesh.pop();
					continue;
				}
			}
			
			// 深度模板
			if (sm->UseDepthStencil())
			{
				mRenderSystem->SetDepthStencilType(sm->GetDepthStencilType());
				mRenderSystem->SetStencilRef(sm->GetStencilRef());
			}

			// 光栅化
			if (sm->UseRasterizer())
			{
				mRenderSystem->SetRasterizerType(sm->GetRasterizerType());
			}

			// 混合
			if (sm->UseBlend())
			{
				mRenderSystem->SetBlendType(sm->GetBlendType());
			}

			// 根据SceneNode设置Shader的相关变量
			Shader* shader = sm->GetShader();
			if (shader)
			{
				// 变换矩阵
				shader->SetWorldViewProj(sm->GetParent()->GetSceneNodeBelongsTo()->GetAbsoluteTransformation() *
					GetActiveCamera()->GetViewMatrix() *
					GetActiveCamera()->GetProjectionMatrix());
				shader->SetWorld(sm->GetParent()->GetSceneNodeBelongsTo()->GetAbsoluteTransformation());
				shader->SetView(GetActiveCamera()->GetViewMatrix());
				shader->SetProj(GetActiveCamera()->GetProjectionMatrix());

				// 材质
				auto& textures = sm->GetTextures();
				for (size_t i = 0; i < textures.size(); i++)
				{
					mRenderSystem->SetTexture(i, textures[i]);
				}
				shader->SetUseTexture(sm->GetTextures().size() > 0);
				shader->SetUseNormalMap(sm->GetTextures().size() > 1);

				// 灯光
				int slotD = 0;
				int slotP = 0;
				int slotS = 0;
				for (size_t i = 0; i < mLightList.size(); i++)
				{
					LIGHT_TYPE lt = mLightList[i]->GetType();
					switch (lt)
					{
					case Soul::LIGHT_TYPE::LT_NONE:
					{
						break;
					}
					case Soul::LIGHT_TYPE::LT_DIRECTIONAL:
					{
						DirectionalLight dl;
						dl.Ambient = mLightList[i]->GetAmbient();
						dl.Diffuse = mLightList[i]->GetDiffuse();
						dl.Specular = mLightList[i]->GetSpecular();
						dl.Direction = mLightList[i]->GetDirection();
						shader->SetDirectinalLight(slotD++, dl);
						break;
					}
					case Soul::LIGHT_TYPE::LT_POINT:
					{
						PointLight pl;
						pl.Ambient = mLightList[i]->GetAmbient();
						pl.Diffuse = mLightList[i]->GetDiffuse();
						pl.Specular = mLightList[i]->GetSpecular();
						pl.Att = mLightList[i]->GetAtt();
						pl.Position = mLightList[i]->GetPosition();
						pl.Range = mLightList[i]->GetRange();
						shader->SetPointLight(slotP++, pl);
						break;
					}
					case Soul::LIGHT_TYPE::LT_SPOT:
					{
						SpotLight sl;
						sl.Spot = mLightList[i]->GetSpot();
						sl.Ambient = mLightList[i]->GetAmbient();
						sl.Diffuse = mLightList[i]->GetDiffuse();
						sl.Specular = mLightList[i]->GetSpecular();
						sl.Att = mLightList[i]->GetAtt();
						sl.Position = mLightList[i]->GetPosition();
						sl.Range = mLightList[i]->GetRange();
						sl.Direction = mLightList[i]->GetDirection();
						shader->SetSpotLight(slotS++, sl);
						break;
					}
					default:
					{
						break;
					}
					}
				}
				// 设置各类灯光的数量
				shader->SetPointLightNum(slotP);
				shader->SetDirectinalLightNum(slotD);
				shader->SetSpotLightNum(slotS);
				// 材质 非阴影
				shader->SetEnableShadow(false);
				shader->SetMaterial(*(sm->GetMaterial()));
				// 设置Shader
				mRenderSystem->BindShader(shader);
				// 渲染
				mRenderSystem->Render(sm->GetRenderParameter());
				// 渲染阴影
				if (sm->IsEnableShadow())
				{
					static Material materialShadow;
					materialShadow.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
					materialShadow.diffuse = { 0.0f, 0.0f, 0.0f, 0.8f };
					materialShadow.specular = { 0.0f, 0.0f, 0.0f, 16.0f };
					shader->SetMaterial(materialShadow);
					shader->SetEnableShadow(true);
					shader->SetShadowMatrix(sm->GetShadowMatrix());
					shader->SetUseTexture(false);
					mRenderSystem->SetDepthStencilType(DepthStencilType::DST_NO_DOUBLE_BLEND);
					mRenderSystem->SetStencilRef(1);
					mRenderSystem->SetBlendType(BlendType::BT_TRANSPARENT);
					mRenderSystem->BindShader(shader);
					// 渲染阴影
					mRenderSystem->Render(sm->GetRenderParameter());
				}
			}
			else
			{
				mRenderSystem->BindShader(nullptr);
			}

			mAllAttachedSubMesh.pop();
		}
		mLightList.clear();
		mCameraNodeList.clear();
		mLightNodeList.clear();
		mRenderableNodeList.clear();
	}

	void SceneManager::Clear()
	{
		RemoveAll();
	}
}