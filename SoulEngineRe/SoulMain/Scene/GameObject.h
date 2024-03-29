#pragma once
#include "../SoulPCH.h"
#include "SubMesh.h"

namespace Soul
{
	class GameObject
	{
	public:
		explicit GameObject(const std::string& name = "GameObject");

		virtual ~GameObject();
		
		const std::string& GetName() const
		{
			return mName;
		}

		void SetVisible(bool isVisible) { mVisible = isVisible; }

		bool IsVisible() { return mVisible; }

		void SetSceneNodeBelongsTo(SceneNode* sceneNode) { mSceneNodeBelongsTo = sceneNode; }

		SceneNode* GetSceneNodeBelongsTo() const { return mSceneNodeBelongsTo; }

		void PushSubMesh(SubMesh* subMesh)
		{ 
			mSunMeshes.push_back(subMesh);
			subMesh->SetParent(this);
		}

		std::vector<SubMesh*>& GetAllSubMesh() { return mSunMeshes; }

		SubMesh* GetSubMesh(const unsigned subMeshIndex)
		{
			if (subMeshIndex < mSunMeshes.size())
			{
				return mSunMeshes[subMeshIndex];
			}
			return nullptr;
		}

		// 获取包围盒，默认不进行世界变换
		BoundingBox GetBoundingBox(bool local = false) const;
		
		// 根据SubMesh构建包围盒
		void BuildBoundingBox();
		
	protected:
		// 物体名称
		std::string mName;
		// 是否可见
		bool mVisible;
		// 由SceneManager管理
		SceneNode* mSceneNodeBelongsTo;
		// 包围盒
		BoundingBox* mBoundingBox;
		// 由自身管理释放
		std::vector<SubMesh*> mSunMeshes;
	};
}