#include "aio_pch.h"
#include "Camera.h"
#include "Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Alexio
{
	Camera::Camera(float left, float right, float bottom, float top)
	{
		mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		mView = glm::mat4x4(1.0f);
		mViewProjection = glm::mat4x4(1.0f);

		mPosition = { 0.0f, 0.0f };
		mRotation = 0.0f;
	}

	void Camera::Update(const glm::mat4x4& model)
	{
		mView = glm::translate(glm::mat4x4(1.0f), glm::vec3(mPosition, 0.0f)) *
			glm::rotate(glm::mat4x4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));

		mViewProjection = mProjection * mView * model;

		Renderer::GetCameraBuffer()->SetData(&mViewProjection, sizeof(glm::mat4x4));
		Renderer::GetCameraBuffer()->Bind(0);
	}
}