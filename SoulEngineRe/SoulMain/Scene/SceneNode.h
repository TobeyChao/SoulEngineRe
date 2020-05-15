#pragma once
#include "../SoulPCH.h"
#include "../Core/Mathematics/SMatrix4x4.h"
#include "../Core/Mathematics/SVector3.h"
#include "../Core/Mathematics/SVector4.h"
namespace Soul
{
	typedef std::vector<SceneNode*> ISceneNodeList;

	class SceneNode
	{
	public:
		SceneNode(SceneNode* parent, SceneManager* mgr, size_t id = -1,
			const Soul::Core::SVector3& position = Soul::Core::SVector3(0, 0, 0),
			const Soul::Core::SVector3& rotation = Soul::Core::SVector3(0, 0, 0),
			const Soul::Core::SVector3& scale = Soul::Core::SVector3(1.0f, 1.0f, 1.0f));

		virtual ~SceneNode();

		virtual void OnRegisterSceneNode();

		virtual void ProcessVisibleGameObject();

		const Soul::Core::SMatrix4x4& GetAbsoluteTransformation() const;

		Soul::Core::SMatrix4x4 GetRelativeTransformation() const;

		size_t GetID() const { return mID; }

		void SetID(size_t id) { mID = id; }

		bool IsRegister() const;

		SceneNode* AddChild(SceneNode* child);

		bool RemoveChild(SceneNode* child);

		void RemoveAllChildren();

		void RemoveFromParent();

		void AttachObj(GameObject* obj);

		void DetachObj();

		void UpdateAbsolutePosition();

		virtual const Soul::Core::SVector3& GetScale() const;

		virtual void SetScale(const Soul::Core::SVector3& scale);

		virtual const Soul::Core::SVector3& GetRotation() const;

		virtual void SetRotation(const Soul::Core::SVector3& rotation);

		virtual const Soul::Core::SVector3& GetPosition() const;

		virtual void SetPosition(const Soul::Core::SVector3& newpos);

		const std::vector<SceneNode*>& GetChildren() const;

		virtual void SetParent(SceneNode* newParent);

		SceneNode* GetParent() const;

		virtual SceneManager* GetSceneManager(void) const;
	protected:

		void SetSceneManager(SceneManager* newManager);

	protected:
		// 节点ID
		size_t mID;
		// 是否已经注册
		bool mIsRegister;
		// Absolute transformation of the node.
		Core::SMatrix4x4 mAbsoluteTransformation;

		// Relative translation of the scene node.
		Core::SVector3 mRelativeTranslation;

		// Relative rotation of the scene node.
		Core::SVector3 mRelativeRotation;

		// Relative scale of the scene node.
		Core::SVector3 mRelativeScale;

		double mData = 0;

		SceneNode* mParent;

		SceneManager* mSceneManager;

		std::vector<SceneNode*> mChildren;

		GameObject* mAttachedGameObject;
	};
}