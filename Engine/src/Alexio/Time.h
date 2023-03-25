#pragma once

#include <chrono>

namespace Alexio
{
	class Time
	{
	public:
		static void Start();
		static void Update();

		inline static float DetlaTime() { return sDeltaTime.count(); }

		inline static float Get() { return sAppTime; }
	private:
		static float sAppTime;
		static std::chrono::duration<float> sDeltaTime;
		static std::chrono::time_point<std::chrono::system_clock> sTimer, sLastFrame;
	};
}