#include "aio_pch.h"
#include "Time.h"

namespace Alexio
{
	std::chrono::duration<float> Time::sDeltaTime;
	std::chrono::time_point<std::chrono::system_clock> Time::sTimer;
	std::chrono::time_point<std::chrono::system_clock> Time::sLastFrame;

	void Time::Start()
	{
		sTimer = std::chrono::system_clock::now();
		sLastFrame = std::chrono::system_clock::now();
	}

	void Time::UpdateDeltaTime()
	{
		sTimer = std::chrono::system_clock::now();
		sDeltaTime = sTimer - sLastFrame;
		sLastFrame = sTimer;
	}

}


