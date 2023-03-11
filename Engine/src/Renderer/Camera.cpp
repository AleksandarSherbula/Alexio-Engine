#include "aio_pch.h"
#include "Camera.h"

namespace Alexio
{
	Camera::Camera(float left, float right, float bottom, float top)
	{
		mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);	
	}
}