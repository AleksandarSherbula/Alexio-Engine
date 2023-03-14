#pragma once

#include "Math/Math.h"
#include "Events/Events.h"

namespace Alexio
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(float aspectRatio);
		
		inline glm::vec2& Position() { return mPosition; }
		inline float& Rotation() { return mRotation; }
		inline float& ZoomLevel() { return mZoomLevel; }

		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseScroll(MouseScrolledEvent& e);

		void OnUpdate(float dt);
		void UpdateProjection();
	private:
		glm::vec2 mPosition;
		float mRotation;
		float mZoomLevel;

		float mAspectRatio;

		glm::mat4x4 mProjection;
		glm::mat4x4 mView;
		glm::mat4x4 mViewProjection;
	};
}