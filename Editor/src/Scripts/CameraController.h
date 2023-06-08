#pragma once
#include "Alexio.h"

namespace Alexio
{
	class CameraController : public EntityScript
	{
	public:
		void OnCreate() override
		{
		}

		void OnUpdate() override
		{
			auto& transform = GetComponent<TransformComponent>();
			float speed = 5.0f;

			if (Input::KeyHeld(LEFT))
				transform.Position.x -= speed * Timer::DeltaTime();
			if (Input::KeyHeld(RIGHT))
				transform.Position.x += speed * Timer::DeltaTime();
			if (Input::KeyHeld(UP))
				transform.Position.y += speed * Timer::DeltaTime();
			if (Input::KeyHeld(DOWN))
				transform.Position.y -= speed * Timer::DeltaTime();
		}

		void OnDestroy() override
		{
		}
	};
}
