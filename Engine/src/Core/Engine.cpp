#include "aio_pch.h"
#include "Engine.h"
#include "Log.h"

#include "Events/AppEvent.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;
	}

	void Engine::Run()
	{		
		OnStart();

		WindowResizeEvent e(1280, 720);
		AIO_LOG_TRACE(e);

		while (true)
		{
			OnUpdate();
		}
	}
}


int main(int argc, char** agrv)
{
	Alexio::Log::Initialize();

	Alexio::Engine* engine = Create();
	engine->Run();
	delete engine;
	return 0;
}


