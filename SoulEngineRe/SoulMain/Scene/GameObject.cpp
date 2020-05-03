#include "GameObject.h"

namespace Soul
{
	GameObject::GameObject(const std::string& name)
		:
		mName(name),
		mSceneNodeBelongsTo(nullptr)
	{
	}
	GameObject::~GameObject()
	{
		for (auto it : mSunMeshs)
		{
			SubMesh* sm = it;
			if (sm)
			{
				delete sm;
				sm = nullptr;
			}
		}
		mSunMeshs.clear();
	}
}