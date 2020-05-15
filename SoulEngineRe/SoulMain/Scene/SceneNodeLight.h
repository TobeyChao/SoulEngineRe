#pragma once
#include "SceneNode.h"

namespace Soul
{
	class SceneNodeLight : public SceneNode
	{
	public:
		SceneNodeLight(SceneNode* parent, SceneManager* mgr, size_t id = -1);
		void OnRegisterSceneNode() override;
		void ProcessVisibleGameObject() override;
	};
}