#include "Engine.h"
#include "Game.h"

#include <memory>

namespace Alexio
{
	void Engine::Run()
	{
		Log::Initialize();
		OnStart();		

		while (true)
		{
			OnUpdate();
		}
	}
}

int main(int argc, char** agrv)
{
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();
	return 0;
}