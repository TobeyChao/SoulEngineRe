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

		float GetNearValue() const { return mZNear; }

		float GetFarValue() const { return mZFar; }

		float GetAspectRatio() const { return mAspect; }

		float GetFov() const { return mFovy; }

		const Core::SVector3& GetForward() const { return mForward; }

		const Core::SVector3& GetRight() const { return mRight; }

		[[nodiscard]] Viewport* GetViewport() const { return mViewport; }

		void SetIsOrthogonal(bool isOrthogonal) { mIsOrthogonal = isOrthogonal; }

		const Core::SMatrix4x4& GetProjectionMatrix() const { return mProjMatrix; }

		const Core::SMatrix4x4& GetViewMatrix() const { return mViewMatrix; }

		void OnRegisterSceneNode() override;

		void ProcessVisibleGameObject() override {};

		void UpdateCamera();

		void RenderScene();

		void SetViewport(Viewport* viewport);

	private:
		void UpdateCameraEyeUpAt();

		void UpdateTransformMatrix();

	private:
		Core::SVector3 mLookAt;
		Core::SVector3 mUp;

		float mFovy;
		float mAspect;
		float mZNear;
		float mZFar;

		Core::SMatrix4x4 mDefaultViewMatrix;
		Core::SMatrix4x4 mViewMatrix;
		Core::SMatrix4x4 mProjMatrix;

		Core::SVector3 mForward;
		Core::SVector3 mRight;

		Core::SVector3 mDefaultForward;
		Core::SVector3 mDefaultRight;

		bool mIsOrthogonal;

		Viewport* mViewport;
	};
}