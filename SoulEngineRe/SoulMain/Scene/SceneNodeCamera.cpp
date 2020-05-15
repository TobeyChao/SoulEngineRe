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
		mFovy(Core::SM_PIDIV4),
		mAspect(4.0f / 3.0f),
		mZNear(0.1f),
		mZFar(1000.0f),
		mForward({ 0.0f, 0.0f, 1.0f }),
		mRight({ 1.0f, 0.0f, 1.0f }),
		mDefaultForward({ 0.0f, 0.0f, 1.0f }),
		mDefaultRight({ 1.0f, 0.0f, 0.0f }),
		mIsOrthogonal(false),
		mViewport(nullptr)
	{
		mDefaultViewMatrix = Core::MatrixLookAtLH(
			Core::SVector3(0.0f, 0.0f, -5.0f),
			Core::SVector3(0.0f, 0.0f, 0.0f),
			Core::SVector3(0.0f, 1.0f, 0.0f));
	}

	void SceneNodeCamera::OnRegisterSceneNode()
	{
		if (mSceneManager->RegisterNode(this, E_SCENENODE_TYPES::EST_CAMERA))
			mIsRegister = true;
		SceneNode::OnRegisterSceneNode();
	}

	void SceneNodeCamera::UpdateCamera()
	{
		// Update Absolute Position
		UpdateAbsolutePosition();
		// Update (Eye, Look, Up)
		UpdateCameraEyeUpAt();
		// Update Proj Mat And View Mat
		UpdateTransformMatrix();
		// Build Frustum
		BuildFrustum(mProjMatrix, mViewMatrix);
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

	void SceneNodeCamera::UpdateTransformMatrix()
	{
		if (mIsOrthogonal)
		{
			if (mViewport)
			{
				mProjMatrix = Core::MatrixOrthographicLH(
					(float)mViewport->GetViewportWidth(), (float)mViewport->GetViewportHeight(), mZNear, mZFar);
			}
			else
			{
				mProjMatrix = Core::Matrix4x4Identity();
			}
		}
		mProjMatrix = Core::MatrixPerspectiveFovLH(mFovy, mAspect, mZNear, mZFar);
		mViewMatrix = Core::MatrixLookAtLH(mRelativeTranslation, mLookAt, mUp);
	}

	void SceneNodeCamera::RenderScene()
	{
		mSceneManager->DrawAll(this, mViewport);
	}

	void SceneNodeCamera::SetViewport(Viewport* viewport)
	{
		mViewport = viewport;
		if (mViewport)
		{
			mAspect = (float)mViewport->GetViewportWidth() / (float)mViewport->GetViewportHeight();
		}
	}
}