#pragma once
#include "SceneNode.h"

namespace Soul
{
	enum class E_SCENENODE_TYPES
	{
		EST_CAMERA,
		EST_SOLID,
		EST_LIGHT
	};

	class SceneManager : public SceneNode
	{
	public:
		SceneManager();

		~SceneManager();

		SceneNode* GetRootSceneNode();

		unsigned int RegisterNode(SceneNode* node, E_SCENENODE_TYPES pass);

		GameObject* CreateGameObject(const std::string& name, ParticleEmitter* particleEmmiter, const json& createParameters);

		GameObject* CreateGameObject(const std::string& name, const json& createParameters);

		GameObject* CreateGameObject(const std::string& name, SIMPLE_GAMEOBJECT simpleGameObject, const json& createParameters);

		GameObject* CreateGameObject(const std::string& name, const std::wstring& meshFilePath, const json& createParameters);

		Light* CreateLight(const std::string& name, LIGHT_TYPE lightType);

		[[nodiscard]] SceneNodeCamera* GetActiveCamera() const;

		void SetActiveCamera(SceneNodeCamera* camera);

		void SetViewport(Viewport* viewport);

		void DrawAll(SceneNodeCamera* camera, Viewport* viewport);

		void EnqueueSubMeshQueue(SubMesh* subMesh);

		void EnqueueLightQueue(Light* light) { mRenderCacheLightList.push_back(light); }

		void ProcessVisibleGameObject() override;

		bool EnableFrustumCulling(bool enable)
		{
			mIsEnableFrustumCulling = enable;
		}
	private:
		void SetCustomEffect(SubMesh* subMesh, const json& effctSetting);

		void DrawAllInShadowQueue(std::queue<SubMesh*>& queue);

		void DrawAllInRefQueue(std::queue<SubMesh*>& queue);

		void DrawAllInQueue(std::queue<SubMesh*>& queue);

		void SetLight(Shader* shader);
	private:
		bool mIsEnableFrustumCulling = false;
		// 需要渲染的所有场景节点
		std::vector<SceneNode*> mCameraNodeList;
		std::vector<SceneNode*> mLightNodeList;
		std::vector<SceneNode*> mRenderableNodeList;
		// current active camera
		SceneNodeCamera* mActiveCamera;
		// 当前的渲染系统
		RenderSystem* mRenderSystem;
		// 当前激活的视口
		Viewport* mActiveViewport;
		// 需要渲染的游戏物体
		std::queue<SubMesh*> mRenderCacheSubMeshesRef;
		std::queue<SubMesh*> mRenderCacheSubMeshesBlendRef;
		std::queue<SubMesh*> mRenderCacheSubMeshesShadowRef;
		std::queue<SubMesh*> mRenderCacheSubMeshes;
		std::queue<SubMesh*> mRenderCacheSubMeshesBlend;
		// 需要设置的灯光list
		std::vector<Light*> mRenderCacheLightList;
		// 创建的所有游戏物体
		std::vector<GameObject*> mGameObjects;
	};
}
