#include "SceneNodeRenderable.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "SubMesh.h"

namespace Soul
{
	SceneNodeRenderable::SceneNodeRenderable(SceneNode* parent, SceneManager* mgr, size_t id)
		:
		SceneNode(parent, mgr, id)
	{}

	void SceneNodeRenderable::OnRegisterSceneNode()
	{
		if (mSceneManager->RegisterNode(this, E_SCENENODE_TYPES::EST_SOLID))
			mIsRegister = true;
		SceneNode::OnRegisterSceneNode();
	}

	void SceneNodeRenderable::ProcessVisibleGameObject()
	{
		UpdateAbsolutePosition();
		if (!mAttachedGameObject || !mAttachedGameObject->IsVisible())
		{
			return;
		}
		for (auto subMesh : mAttachedGameObject->GetAllSubMesh())
		{
			mSceneManager->EnqueueSubMeshQueue(subMesh);
		}
	}
}
