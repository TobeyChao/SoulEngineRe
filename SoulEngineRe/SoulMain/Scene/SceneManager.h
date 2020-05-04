#pragma once
#include "SceneNode.h"

namespace Soul
{
	enum class E_SCENENODE_TYPES
	{
		EST_SKYBOX,
		EST_CAMERA,
		EST_SOLID,
		EST_LIGHT
	};

	class SceneManager : public SceneNode
	{
	public:
		SceneManager();

		~SceneManager();

		/***************************************************/
		SceneNodeCamera* AddCameraSceneNode(SceneNode* parent = nullptr,
			const Core::SVector3& position = Core::SVector3(0.f, 0.f, -10.0f),
			const Core::SVector3& lookAt = Core::SVector3(0.f, 0.f, 0.f),
			size_t id = -1, bool makeActive = true);

		virtual SceneNode* AddSkyBoxSceneNode(ITexture* texture, SceneNode* parent = nullptr, size_t id = -1);

		virtual SceneNode* GetRootSceneNode();

		virtual SceneNode* GetSceneNodeFromId(int id, SceneNode* start = nullptr);

		virtual SceneNode* GetSceneNodeFromName(const std::wstring& name, SceneNode* start = nullptr);

		virtual unsigned int RegisterNode(SceneNode* node, E_SCENENODE_TYPES pass);

		virtual GameObject* CreateGameObject(const std::string& name, ParticleEmitter* particleEmmiter);

		virtual GameObject* CreateGameObject(const std::string& name, SIMPLE_GAMEOBJECT simpleGameObject, const json& createParameters);

		virtual GameObject* CreateGameObject(const std::string& name, const std::wstring& meshFilePath, const json& createParameters);

		virtual Light* CreateLight(const std::string& name, LIGHT_TYPE lightType);

		[[nodiscard]] virtual SceneNodeCamera* GetActiveCamera() const;

		virtual void SetActiveCamera(SceneNodeCamera* camera);

		virtual void SetViewport(Viewport* viewport);

		virtual void DrawAll(SceneNodeCamera* camera, Viewport* viewport);

		virtual void Clear();
		/********************************************/

		void Render() override;

		void EnqueueSubMeshQueue(SubMesh* rp)
		{
			mAllAttachedGameObject.push(rp);
		}

		void EnqueueLightQueue(Light* light)
		{
			mLightList.push_back(light);
		}

	private:
		void SetCustomEffect(SubMesh* subMesh, const json& effctSetting);

	private:

		std::vector<SceneNode*> mCameraNodeList;
		std::vector<SceneNode*> mSkyBoxNodeList;
		std::vector<SceneNode*> mLightNodeList;
		std::vector<SceneNode*> mSolidNodeList;

		// current active camera
		SceneNodeCamera* mActiveCamera;

		// 当前的渲染系统
		RenderSystem* mRenderSystem;

		// 当前激活的视口
		Viewport* mActiveViewport;

		// 需要渲染的游戏物体
		std::queue<SubMesh*> mAllAttachedGameObject;

		// 需要设置的灯光list
		std::vector<Light*> mLightList;
	};
}
