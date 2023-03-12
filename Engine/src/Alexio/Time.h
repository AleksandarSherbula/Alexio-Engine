#pragma once

#include <chrono>

namespace Alexio
{
	class Time
	{
	public:
		static void Start();
		static void UpdateDeltaTime();

		inline static float DetlaTime() { return sDeltaTime.count(); }
	private:
		static std::chrono::duration<float> sDeltaTime;
		static std::chrono::time_point<std::chrono::system_clock> sTimer, sLastFrame;
	};
}