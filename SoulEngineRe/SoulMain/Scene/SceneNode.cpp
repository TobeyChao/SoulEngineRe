#include "SceneNode.h"
#include "SceneManager.h"
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
		mRelativeTranslation(position),
		mRelativeRotation(rotation),
		mRelativeScale(scale),
		mParent(nullptr),
		mSceneManager(mgr)
	{
		if (parent)
			parent->AddChild(this);
		UpdateAbsolutePosition();
	}

	SceneNode::~SceneNode()
	{
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
		ISceneNodeList::iterator it = mChildren.begin();
		for (; it != mChildren.end(); ++it)
		{
			(*it)->OnRegisterSceneNode();
		}
	}

	void SceneNode::ProcessVisibleGameObject()
	{

	}

	const Core::SMatrix4x4& SceneNode::GetAbsoluteTransformation() const
	{
		return mAbsoluteTransformation;
	}

	Core::SMatrix4x4 SceneNode::GetRelativeTransformation() const
	{
		Core::SMatrix4x4 scaleMatrix =
			Core::MatrixScaling(mRelativeScale.x, mRelativeScale.y, mRelativeScale.z);
		Core::SMatrix4x4 rotateMatrix =
			Core::MatrixRotationX(mRelativeRotation.x) *
			Core::MatrixRotationY(mRelativeRotation.y) *
			Core::MatrixRotationZ(mRelativeRotation.z);
		Core::SMatrix4x4 translateMatrix = Core::MatrixTranslation(
			mRelativeTranslation.x, mRelativeTranslation.y, mRelativeTranslation.z);

		return scaleMatrix * rotateMatrix * translateMatrix;
	}

	bool SceneNode::IsRegister() const
	{
		return mIsRegister;
	}

	SceneNode* SceneNode::AddChild(SceneNode* child)
	{
		if (child && (child != this))
		{
			if (mSceneManager != child->mSceneManager)
				child->SetSceneManager(mSceneManager);
			child->RemoveFromParent();
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
				(*it)->mParent = nullptr;
				mChildren.erase(it);
				return true;
			}
		}
		return false;
	}

	void SceneNode::RemoveAllChildren()
	{
		ISceneNodeList::iterator it = mChildren.begin();
		for (; it != mChildren.end(); ++it)
		{
			(*it)->mParent = nullptr;
		}
		mChildren.clear();
	}

	void SceneNode::RemoveFromParent()
	{
		if (mParent)
			mParent->RemoveChild(this);
	}

	void SceneNode::AttachObj(GameObject* obj)
	{
		mAttachedGameObject = obj;
		obj->SetSceneNodeBelongsTo(this);
	}

	void SceneNode::DetachObj()
	{
		mAttachedGameObject = nullptr;
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
		RemoveFromParent();
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