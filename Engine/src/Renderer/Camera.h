#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Buffer.h"

namespace Alexio
{
	class Camera
	{
	public:
		Camera(float left, float right, float bottom, float top);

		inline glm::mat4x4 GetProjection() { return mProjection; }
	private:
		glm::mat4x4 mProjection;
	public:
		static Scope<ConstantBuffer> sProjectionBuffer;
	};
}