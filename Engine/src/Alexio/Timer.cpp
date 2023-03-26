#include "aio_pch.h"
#include "Timer.h"

namespace Alexio
{
	float Timer::sAppTime = 0.0f;
	std::chrono::duration<float> Timer::sDeltaTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Timer::sAppStartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Timer::sLastFrame;

	Timer::Timer()
	{
		mStart = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		auto stop = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStart).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(stop).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;
		AIO_LOG_TRACE("{0}us ({1}ms)", duration, ms);
	}

	void Timer::StartApp()
	{
		sAppStartTime = std::chrono::high_resolution_clock::now();
		sLastFrame = std::chrono::high_resolution_clock::now();
	}

	void Timer::Update()
	{
		sAppStartTime = std::chrono::high_resolution_clock::now();
		sDeltaTime = sAppStartTime - sLastFrame;
		sLastFrame = sAppStartTime;

		sAppTime += sDeltaTime.count();
	}
}


