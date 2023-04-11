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
		
		inline Vector2& Position() { return mPosition; }
		inline float& Rotation() { return mRotation; }
		inline float& ZoomLevel() { return mZoomLevel; }

		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseScroll(MouseScrolledEvent& e);

		void OnUpdate(float dt);
		void UpdateProjection(float width, float height);

		void OnResize(float width, float height);
	private:
		Vector2 mPosition;
		float mRotation;
		float mZoomLevel;

		float mAspectRatio;

		Mat4x4 mProjection;
		Mat4x4 mView;
		Mat4x4 mViewProjection;
	};
}