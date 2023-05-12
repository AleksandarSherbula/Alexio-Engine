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
			auto& transform = GetComponent<TransformComponent>().Transform;
			float speed = 5.0f;

			if (Input::KeyHeld(LEFT))
				transform[3][0] -= speed * Timer::DeltaTime();
			if (Input::KeyHeld(RIGHT))
				transform[3][0] += speed * Timer::DeltaTime();
			if (Input::KeyHeld(UP))
				transform[3][1] += speed * Timer::DeltaTime();
			if (Input::KeyHeld(DOWN))
				transform[3][1] -= speed * Timer::DeltaTime();
		}

		void OnDestroy() override
		{
		}
	};
}
