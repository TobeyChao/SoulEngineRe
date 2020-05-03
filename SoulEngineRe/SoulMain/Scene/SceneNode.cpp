#include "SceneNode.h"
#include "GameObject.h"

namespace Soul
{
	SceneNode::SceneNode(SceneNode* parent, SceneManager* mgr, size_t id,
		const Core::SVector3& position,
		const Core::SVector3& rotation,
		const Core::SVector3& scale)
		:
		mID(0),
		mIsRegister(false),
		mIsVisible(true),
		mRelativeTranslation(position),
		mRelativeRotation(rotation),
		mRelativeScale(scale),
		mParent(nullptr),
		mSceneManager(mgr)
	{
		if (parent)
			parent->AddChild(this);

		SceneNode::UpdateAbsolutePosition();
	}

	SceneNode::~SceneNode()
	{
		for (auto it : mAttachedGameObject)
		{
			GameObject* go = it;
			if (go)
			{
				delete go;
				go = nullptr;
			}
		}
		mAttachedGameObject.clear();
		
		for (auto it : mChildren)
		{
			SceneNode* sn = it;
			if (sn)
			{
				delete sn;
				sn = nullptr;
			}
		}
		mChildren.clear();
	}

	void SceneNode::OnRegisterSceneNode()
	{
		if (mIsVisible)
		{
			ISceneNodeList::iterator it = mChildren.begin();
			for (; it != mChildren.end(); ++it)
				(*it)->OnRegisterSceneNode();
		}
	}
	std::wstring SceneNode::GetName() const
	{
		return mName;
	}
	void SceneNode::SetName(std::wstring name)
	{
		mName = name;
	}
	const Core::SMatrix4x4& SceneNode::GetAbsoluteTransformation() const
	{
		return mAbsoluteTransformation;
	}
	Core::SMatrix4x4 SceneNode::GetRelativeTransformation() const
	{
		Core::SMatrix4x4 scaleMatrix =
			Core::MatrixScaling(mRelativeScale.x, mRelativeScale.y, mRelativeScale.z);
		//Core::SMatrix4x4 rotateMatrix = 
		//	Core::MatrixRotationRollPitchYaw(mRelativeRotation.x, mRelativeRotation.y, mRelativeRotation.z);
		Core::SMatrix4x4 rotateMatrix =
			Core::MatrixRotationX(mRelativeRotation.x) *
			Core::MatrixRotationY(mRelativeRotation.y) *
			Core::MatrixRotationZ(mRelativeRotation.z);
		Core::SMatrix4x4 translateMatrix = Core::MatrixTranslation(
			mRelativeTranslation.x, mRelativeTranslation.y, mRelativeTranslation.z);

		return scaleMatrix * rotateMatrix * translateMatrix;
	}
	bool SceneNode::IsVisible() const
	{
		return mIsVisible;
	}
	void SceneNode::SetVisible(bool isVisible)
	{
		mIsVisible = isVisible;
	}
	size_t SceneNode::GetID() const
	{
		return mID;
	}
	void SceneNode::SetID(size_t id)
	{
		mID = id;
	}
	bool SceneNode::IsRegister() const
	{
		return mIsRegister;
	}
	SceneNode* SceneNode::AddChild(SceneNode* child)
	{
		if (child && (child != this))
		{
			// Change scene manager?
			if (mSceneManager != child->mSceneManager)
				child->SetSceneManager(mSceneManager);
			child->Remove(); // remove from old parent
			mChildren.push_back(child);
			child->mParent = this;
		}
		return child;
	}
	bool SceneNode::RemoveChild(SceneNode* child)
	{
		ISceneNodeList::iterator it = mChildren.begin();
		for (; it != mChildren.end(); ++it)
		{
			if ((*it) == child)
			{
				(*it)->mParent = 0;
				mChildren.erase(it);
				return true;
			}
		}
		return false;
	}
	void SceneNode::RemoveAll()
	{
		ISceneNodeList::iterator it = mChildren.begin();
		for (; it != mChildren.end(); ++it)
		{
			(*it)->mParent = nullptr;
		}

		mChildren.clear();
	}
	void SceneNode::Remove()
	{
		if (mParent)
			mParent->RemoveChild(this);
	}
	void SceneNode::AttachObj(GameObject* obj)
	{
		mAttachedGameObject.push_back(obj);
		obj->SetSceneNodeBelongsTo(this);
	}
	void SceneNode::DetachObj(GameObject* obj)
	{
		auto it = std::find(mAttachedGameObject.begin(), mAttachedGameObject.end(), obj);
		if (it != mAttachedGameObject.end())
		{
			obj->SetSceneNodeBelongsTo(nullptr);
			mAttachedGameObject.erase(it);
		}
	}
	void SceneNode::UpdateAbsolutePosition()
	{
		if (mParent)
		{
			mAbsoluteTransformation = GetRelativeTransformation() * mParent->GetAbsoluteTransformation();
		}
		else
		{
			mAbsoluteTransformation = GetRelativeTransformation();
		}
	}
	const Core::SVector3& SceneNode::GetScale() const
	{
		return mRelativeScale;
	}
	void SceneNode::SetScale(const Core::SVector3& scale)
	{
		mRelativeScale = scale;
	}
	const Core::SVector3& SceneNode::GetRotation() const
	{
		return mRelativeRotation;
	}
	void SceneNode::SetRotation(const Core::SVector3& rotation)
	{
		mRelativeRotation = rotation;
	}
	const Core::SVector3& SceneNode::GetPosition() const
	{
		return mRelativeTranslation;
	}
	void SceneNode::SetPosition(const Core::SVector3& newpos)
	{
		mRelativeTranslation = newpos;
	}
	const std::vector<SceneNode*>& SceneNode::GetChildren() const
	{
		return mChildren;
	}
	void SceneNode::SetParent(SceneNode* newParent)
	{
		Remove();
		mParent = newParent;
		if (mParent)
			mParent->AddChild(this);
	}
	SceneNode* SceneNode::GetParent() const
	{
		return mParent;
	}
	SceneManager* SceneNode::GetSceneManager(void) const
	{
		return mSceneManager;
	}
	void SceneNode::SetSceneManager(SceneManager* newManager)
	{
		mSceneManager = newManager;

		ISceneNodeList::iterator it = mChildren.begin();
		for (; it != mChildren.end(); ++it)
			(*it)->SetSceneManager(newManager);
	}
}