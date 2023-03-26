#include "aio_pch.h"
#include "Camera.h"
#include "Alexio/Timer.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	Camera::Camera(float aspectRatio)
	{		
		mAspectRatio = aspectRatio;
		mZoomLevel = 1.0f;

		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
		mView = glm::mat4x4(1.0f);
		mViewProjection = mProjection * mView;

		mPosition = { 0.0f, 0.0f };
		mRotation = 0.0f;
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(AIO_BIND_EVENT_FN(Camera::OnWindowResize));
		dispatcher.Dispatch<MouseScrolledEvent>(AIO_BIND_EVENT_FN(Camera::OnMouseScroll));
	}

	bool Camera::OnWindowResize(WindowResizeEvent& e)
	{
		mAspectRatio = static_cast<float>(e.GetWidth()) / e.GetHeight();
		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
		return false;
	}

	bool Camera::OnMouseScroll(MouseScrolledEvent& e)
	{
		mZoomLevel -= e.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mProjection = glm::ortho(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, mZoomLevel, -mZoomLevel);
		return false;
	}

	void Camera::OnUpdate(float dt)
	{
		float moveSpeed = mZoomLevel;
		
		if (Alexio::Input::KeyHeld(A))
			mPosition.x -= moveSpeed * Timer::DetlaTime();
		if (Alexio::Input::KeyHeld(D))
			mPosition.x += moveSpeed * Timer::DetlaTime();
		if (Alexio::Input::KeyHeld(W))
			mPosition.y -= moveSpeed * Timer::DetlaTime();
		if (Alexio::Input::KeyHeld(S))
			mPosition.y += moveSpeed * Timer::DetlaTime();

		mView = glm::translate(glm::mat4x4(1.0f), glm::vec3(mPosition, 0.0f)) *
			glm::rotate(glm::mat4x4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));

		//mView = glm::inverse(transform);
		mViewProjection = mProjection * mView;

		Renderer::GetCameraBuffer()->SetData(&mViewProjection, sizeof(glm::mat4x4));
		Renderer::GetCameraBuffer()->Bind(0);
	}

	void Camera::UpdateProjection()
	{
		
	}
}