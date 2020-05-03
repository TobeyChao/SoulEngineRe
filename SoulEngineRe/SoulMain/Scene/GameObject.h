#pragma once
#include "../SoulPCH.h"
#include "SubMesh.h"

namespace Soul
{
	class GameObject
	{
	public:
		GameObject(const std::string& name);

		virtual ~GameObject();
		
		void SetSceneNodeBelongsTo(SceneNode* sceneNode)
		{
			mSceneNodeBelongsTo = sceneNode;
		}

		SceneNode* GetSceneNodeBelongsTo()
		{
			return mSceneNodeBelongsTo;
		}

		void PushSubMesh(SubMesh* subMesh)
		{
			mSunMeshs.push_back(subMesh);
			subMesh->SetParent(this);
		}

		std::vector<SubMesh*>& GetAllSubMesh()
		{
			return mSunMeshs;
		}

		SubMesh* GetSubMesh(unsigned subMeshIndex)
		{
			if (subMeshIndex < mSunMeshs.size())
			{
				return mSunMeshs[subMeshIndex];
			}
			return nullptr;
		}

	protected:
		std::string mName;
		// 由SceneManager管理
		SceneNode* mSceneNodeBelongsTo;
		// 由自身管理释放
		std::vector<SubMesh*> mSunMeshs;
	};
}