#include "GameObject.h"
#include "BoundingBox.h"
#include "SceneNodeCamera.h"

namespace Soul
{
	GameObject::GameObject(const std::string& name)
		:
		mName(name),
		mSceneNodeBelongsTo(nullptr),
		mBoundingBox(nullptr)
	{
	}

	GameObject::~GameObject()
	{
		for (auto it : mSunMeshes)
		{
			auto sm = it;
			if (sm)
			{
				delete sm;
				sm = nullptr;
			}
		}
		mSunMeshes.clear();
	}
	BoundingBox GameObject::GetBoundingBox(bool local) const
	{
		// 没有绑定的场景Node也会返回局部坐标系包围盒
		if (local || !mSceneNodeBelongsTo)
		{
			return *mBoundingBox;
		}
		BoundingBox out;
		if (mBoundingBox)
		{
			mBoundingBox->Transform(out, mSceneNodeBelongsTo->GetAbsoluteTransformation());
		}
		return out;
	}
	void GameObject::BuildBoundingBox()
	{
		// 没Mesh不构建
		if (mSunMeshes.empty())
		{
			return;
		}
		// Mesh应该有最大和最小点属性
		mBoundingBox = new BoundingBox();
		const auto meshData = mSunMeshes[0]->GetOriginalMeshDataPtr();
		BoundingBox::CreateFromPoints(*mBoundingBox, meshData->Min, meshData->Max);
	}
}