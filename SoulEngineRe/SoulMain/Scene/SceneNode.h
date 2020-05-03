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

		virtual std::wstring GetName() const;

		virtual void SetName(std::wstring name);

		virtual void Render() {};

		virtual const Soul::Core::SMatrix4x4& GetAbsoluteTransformation() const;

		virtual Soul::Core::SMatrix4x4 GetRelativeTransformation() const;

		virtual bool IsVisible() const;

		virtual void SetVisible(bool isVisible);

		virtual size_t GetID() const;

		virtual void SetID(size_t id);

		virtual bool IsRegister() const;

		virtual SceneNode* AddChild(SceneNode* child);

		virtual bool RemoveChild(SceneNode* child);

		virtual void RemoveAll();

		virtual void Remove();

		virtual void AttachObj(GameObject* obj);

		virtual void DetachObj(GameObject* obj);

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
		size_t mID;

		bool mIsRegister;

		std::wstring mName;

		bool mIsVisible;

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

		std::vector<GameObject*> mAttachedGameObject;
	};
}