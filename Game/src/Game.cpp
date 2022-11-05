#include "Alexio.h"

class Game : public Alexio::Engine
{
public:
	Game::Game()
	{
		
	}

	void Game::OnStart() override
	{
		AIO_LOG_INFO("Hello! Welcome to Alexio Game Engine");
		AIO_LOG_TRACE("Game has been traced and logged with macro");
		AIO_LOG_WARN("Game has a warning and logged with macro");
		AIO_LOG_ERROR("Game has a error and logged with macro");
		AIO_LOG_CRITICAL("Game has a critical and logged with macro");
	}

	void Game::OnUpdate() override
	{

	}
};

Alexio::Engine* Create()
{
	return new Game();
}