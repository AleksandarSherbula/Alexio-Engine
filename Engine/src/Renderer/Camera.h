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
		
		inline glm::vec2 GetPosition() { return mPosition; }
		inline void SetPosition(const glm::vec2& position) { mPosition = position; }

		inline float GetRotation() { return mRotation; }
		inline void SetRotation(float rotation) { mRotation = rotation; }

		void Update(const glm::mat4x4& model);
	private:
		glm::mat4x4 mProjection;
		glm::mat4x4 mView;
		glm::mat4x4 mViewProjection;

		glm::vec2 mPosition;
		float mRotation;
	};
}