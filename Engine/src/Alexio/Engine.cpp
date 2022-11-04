#include "aio_pch.h"
#include "Engine.h"
#include "Game.h"

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

		while (true)
		{
			OnUpdate();
		}
	}
}

int main(int argc, char** agrv)
{
	Alexio::Log::Initialize();

	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();	
	return 0;
}