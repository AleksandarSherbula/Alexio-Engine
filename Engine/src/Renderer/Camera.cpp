#include "aio_pch.h"
#include "Camera.h"

namespace Alexio
{
	Camera::Camera(float left, float right, float bottom, float top)
	{
		mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		unsigned int uboProjection;

		glCreateBuffers(1, &uboProjection);
		glNamedBufferData(uboProjection, sizeof(glm::mat4x4), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboProjection);
		
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4x4), glm::value_ptr(mProjection));
	}
}