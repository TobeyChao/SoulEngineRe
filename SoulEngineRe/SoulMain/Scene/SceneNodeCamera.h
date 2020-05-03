#pragma once
#include "SceneNode.h"
#include "Frustum.h"

namespace Soul
{
	class SceneNodeCamera : public SceneNode, public Frustum
	{
	public:
		SceneNodeCamera(SceneNode* parent, SceneManager* mgr, size_t id,
			const Core::SVector3& position = Core::SVector3(0.0f, 0.0f, 0.0f),
			const Core::SVector3& lookat = Core::SVector3(0.0f, 0.0f, 0.0f));

		void SetProjectionMatrix(const Core::SMatrix4x4& projection);

		void SetIsOrthogonal(bool isOrthogonal);

		const Core::SMatrix4x4& GetProjectionMatrix() const;

		const Core::SMatrix4x4& GetViewMatrix(bool defaultCam = false) const;

		float GetNearValue() const;

		float GetFarValue() const;

		float GetAspectRatio() const;

		float GetFov() const;

		void OnRegisterSceneNode() override;

		void Render() override;

		void SetViewport(Viewport* viewport)
		{
			mViewport = viewport;
			UpdateCamera();
		}
		const Core::SVector3& GetForward() const
		{
			return mForward;
		}
		const Core::SVector3& GetRight() const
		{
			return mRight;
		}
		[[nodiscard]] Viewport* GetViewport() const
		{
			return mViewport;
		}
	protected:
		void UpdateCamera();

		void UpdateCameraEyeUpAt();

		void UpdateTransformMatrix();

	protected:
		Core::SVector3 mLookAt;
		Core::SVector3 mUp;

		float mFovy;
		float mAspect;
		float mZNear;
		float mZFar;

		float mMoveSpeed;

		Core::SMatrix4x4 mDefaultViewMatrix;
		Core::SMatrix4x4 mViewMatrix;
		Core::SMatrix4x4 mProjMatrix;

		Core::SVector3 mForward;
		Core::SVector3 mRight;

		Core::SVector3 mDefaultLookAt;
		Core::SVector3 mDefaultForward;
		Core::SVector3 mDefaultRight;

		bool mIsOrthogonal;

		Viewport* mViewport;
	};
}