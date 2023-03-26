#include "aio_pch.h"
#include "Timer.h"

namespace Alexio
{
	float Timer::sAppTime = 0.0f;
	std::chrono::duration<float> Timer::sDeltaTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Timer::sCurrentTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Timer::sLastFrame;

	Timer::Timer()
	{
		mStartPoint = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		mEndPoint = std::chrono::high_resolution_clock::now();

		float start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartPoint).time_since_epoch().count();
		float end = std::chrono::time_point_cast<std::chrono::microseconds>(mEndPoint).time_since_epoch().count();

		float duration = end - start;
		double ms = duration * 0.001;
		AIO_LOG_TRACE("{0}us ({1}ms)", duration, ms);
	}

	void Timer::StartApp()
	{
		sCurrentTime = std::chrono::high_resolution_clock::now();
		sLastFrame = std::chrono::high_resolution_clock::now();
	}

	void Timer::Update()
	{
		sCurrentTime = std::chrono::high_resolution_clock::now();
		sDeltaTime = sCurrentTime - sLastFrame;
		sLastFrame = sCurrentTime;
		sAppTime += sDeltaTime.count();
	}
}


