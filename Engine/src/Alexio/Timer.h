#pragma once

#include <chrono>

namespace Alexio
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		static void StartApp();
		static void Update();

		static float Get() { return sAppTime; }
		inline static float DeltaTime() { return sDeltaTime.count(); }
	private:
		static float sAppTime;
		static std::chrono::duration<float> sDeltaTime;
		static std::chrono::time_point<std::chrono::high_resolution_clock> sCurrentTime, sLastFrame;

		std::chrono::time_point<std::chrono::high_resolution_clock> mStartPoint, mEndPoint;
	};
}