#include "SceneNodeCamera.h"
#include "../Core/Mathematics/MathConsts.h"
#include "../Core/Mathematics/SVector3.h"
#include "../RenderSystem/Viewport.h"
#include "SceneManager.h"

namespace Soul
{
	SceneNodeCamera::SceneNodeCamera(SceneNode* parent, SceneManager* mgr, size_t id,
		const Core::SVector3& position, const Core::SVector3& lookat)
		:
		SceneNode(parent, mgr, id, position, Core::SVector3(0, 0, 0), Core::SVector3(1.0f, 1.0f, 1.0f)),
		mLookAt(lookat),
		mUp(Core::SVector3(0.0f, 1.0f, 0.0f)),
		mForward({ 0.0f, 0.0f, 1.0f }),
		mRight({ 1.0f, 0.0f, 1.0f }),
		mIsOrthogonal(false)
	{
		mFovy = Core::SM_PIDIV4;

		mAspect = 4.0f / 3.0f;

		mZNear = 0.1f;
		mZFar = 1000.0f;

		mMoveSpeed = 0.5f;

		mDefaultForward = Core::SVector3(0.0f, 0.0f, 1.0f);
		mDefaultRight = Core::SVector3(1.0f, 0.0f, 0.0f);
	}

	void SceneNodeCamera::SetProjectionMatrix(const Core::SMatrix4x4& projection)
	{
	}

	void SceneNodeCamera::SetIsOrthogonal(bool isOrthogonal)
	{
		mIsOrthogonal = isOrthogonal;
	}

	const Core::SMatrix4x4 SceneNodeCamera::GetProjectionMatrix() const
	{
		if (mIsOrthogonal)
		{
			if (mViewport)
			{
				return Core::MatrixOrthographicLH(
				(float)mViewport->GetViewportWidth(), (float)mViewport->GetViewportHeight(), mZNear, mZFar);
			}
			else
			{
				return Core::Matrix4x4Identity();
			}
		}
		return Core::MatrixPerspectiveFovLH(mFovy, mAspect, mZNear, mZFar);
	}

	const Core::SMatrix4x4 SceneNodeCamera::GetViewMatrix(bool defaultCam) const
	{
		if (defaultCam)
		{
			return Core::MatrixLookAtLH(
				Core::SVector3(0.0f, 0.0f, -10.0f),
				Core::SVector3(0.0f, 0.0f, 0.0f),
				Core::SVector3(0.0f, 1.0f, 0.0f));
		}
		return Core::MatrixLookAtLH(mRelativeTranslation, mLookAt, mUp);
	}

	float SceneNodeCamera::GetNearValue() const
	{
		return mZNear;
	}

	float SceneNodeCamera::GetFarValue() const
	{
		return mZFar;
	}

	float SceneNodeCamera::GetAspectRatio() const
	{
		return mAspect;
	}

	float SceneNodeCamera::GetFov() const
	{
		return mFovy;
	}

	void SceneNodeCamera::OnRegisterSceneNode()
	{
		if (!mIsRegister)
		{
			if (mSceneManager->RegisterNode(this, E_SCENENODE_TYPES::EST_CAMERA))
				mIsRegister = true;
		}

		SceneNode::OnRegisterSceneNode();
	}

	void SceneNodeCamera::UpdateCameraEyeUpAt()
	{
		//通过mPitch, mYaw, mRoll重新计算相机的lookat eyeposition up
		Core::SMatrix4x4 rotateMatrix =
			Core::MatrixRotationRollPitchYaw(mRelativeRotation.x, mRelativeRotation.y, mRelativeRotation.z);

		//计算lookat
		mLookAt = mDefaultForward * rotateMatrix;
		mLookAt.Normalize();

		//计算up 默认右方x轴正方向 前方为z轴正方向 乘上旋转矩阵得到新的方向 叉乘即为up
		mRight = mDefaultRight * rotateMatrix;
		mForward = mDefaultForward * rotateMatrix;
		mUp = Core::Cross(mForward, mRight);

		//计算视点
		mLookAt = mRelativeTranslation + mLookAt;
	}

	void SceneNodeCamera::Render()
	{
		UpdateAbsolutePosition();
		UpdateCameraEyeUpAt();
		// 调用SceneManager.Render(); 
		mSceneManager->DrawAll(this, mViewport);
	}

	void SceneNodeCamera::UpdateCamera()
	{
		if (mViewport)
		{
			mAspect = (float)mViewport->GetViewportWidth() / (float)mViewport->GetViewportHeight();
		}
	}
}