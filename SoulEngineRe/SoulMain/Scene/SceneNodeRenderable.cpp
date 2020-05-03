#include "SceneNodeRenderable.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "SubMesh.h"

namespace Soul
{
	SceneNodeRenderable::SceneNodeRenderable(SceneNode* parent, SceneManager* mgr, size_t id)
		:
		SceneNode(parent, mgr, id)
	{
	}
	void SceneNodeRenderable::OnRegisterSceneNode()
	{
		if (mIsVisible)
		{
			if (!mIsRegister)
			{
				if (mSceneManager->RegisterNode(this, E_SCENENODE_TYPES::EST_SOLID))
					mIsRegister = true;
			}
		}
		SceneNode::OnRegisterSceneNode();
	}
	void SceneNodeRenderable::Render()
	{
		UpdateAbsolutePosition();
		for (auto& gameObject : mAttachedGameObject)
		{
			for (auto subMesh : gameObject->GetAllSubMesh())
			{
				mSceneManager->EnqueueSubMeshQueue(subMesh);
			}
		}
	}
}
