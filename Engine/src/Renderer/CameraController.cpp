#include "aio_pch.h"
#include "CameraController.h"
#include "Alexio/Timer.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	CameraController::CameraController(float aspectRatio)
	{		
		mAspectRatio = aspectRatio;
		mZoomLevel = 1.0f;
		
		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
		mView = Mat4x4(1.0f);
		mViewProjection = mProjection * mView;

		mPosition = { 0.0f, 0.0f };
		mRotation = 0.0f;
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(AIO_BIND_EVENT_FN(CameraController::OnWindowResize));
		dispatcher.Dispatch<MouseScrolledEvent>(AIO_BIND_EVENT_FN(CameraController::OnMouseScroll));
	}

	bool CameraController::OnWindowResize(WindowResizeEvent& e)
	{
		mAspectRatio = static_cast<float>(e.GetWidth()) / e.GetHeight();
		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
		return false;
	}

	bool CameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		mZoomLevel -= e.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
		return false;
	}

	void CameraController::OnUpdate()
	{
		float moveSpeed = mZoomLevel;
		
		if (Alexio::Input::KeyHeld(LEFT))
			mPosition.x -= moveSpeed * Timer::DeltaTime();
		if (Alexio::Input::KeyHeld(RIGHT))
			mPosition.x += moveSpeed * Timer::DeltaTime();
		if (Alexio::Input::KeyHeld(UP))
			mPosition.y -= moveSpeed * Timer::DeltaTime();
		if (Alexio::Input::KeyHeld(DOWN))
			mPosition.y += moveSpeed * Timer::DeltaTime();

		AIO_LOG_TRACE("CameraController position: {0} : {1}", mPosition.x, mPosition.y);

		Mat4x4 transform = glm::translate(Mat4x4(1.0f), Vector3(mPosition, 0.0f)) *
			glm::rotate(Mat4x4(1.0f), glm::radians(mRotation), Vector3(0, 0, 1));
		
		mView = glm::inverse(transform);
		mViewProjection = mProjection * mView;

		Renderer::GetCameraBuffer()->SetData(&mViewProjection, sizeof(Mat4x4));
		Renderer::GetCameraBuffer()->Bind(0);
	}

	void CameraController::UpdateProjection(float width, float height)
	{
		mAspectRatio = width / height;
		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
	}

	void CameraController::OnResize(float width, float height)
	{

	}
}