#include "aio_pch.h"
#include "Camera.h"

namespace Alexio
{
	Scope<ConstantBuffer> Camera::sProjectionBuffer = nullptr;

	Camera::Camera(float left, float right, float bottom, float top)
	{
		mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		sProjectionBuffer = ConstantBuffer::Create(sizeof(glm::mat4x4), 0);
		sProjectionBuffer->SetData(glm::value_ptr(mProjection), sizeof(glm::mat4x4), 0);
	}
}