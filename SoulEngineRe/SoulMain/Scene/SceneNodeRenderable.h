#pragma once
#include "SceneNode.h"

namespace Soul
{
	class SceneNodeRenderable : public SceneNode
	{
	public:
		SceneNodeRenderable(SceneNode* parent, SceneManager* mgr, size_t id = -1);
		void OnRegisterSceneNode() override;
		void ProcessVisibleGameObject() override;
	};
}