#include "Alexio.h"

class Game : public Alexio::Engine
{
public:
	Game::Game()
	{
		
	}

	bool Game::OnStart() override
	{
		AIO_LOG_INFO("Hello! Welcome to Alexio Game Engine {0}:", sizeof(int));
		AIO_LOG_TRACE("Game has been traced and logged with macro");
		AIO_LOG_WARN("Game has a warning and logged with macro");
		AIO_LOG_ERROR("Game has a error and logged with macro");
		AIO_LOG_CRITICAL("Game has a critical and logged with macro");

		return true;
	}

	bool Game::OnUpdate() override
	{
		if (Alexio::Input::MouseButtonPressed(Alexio::L_BUTTON))
			AIO_LOG_TRACE("L_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::L_BUTTON))
			AIO_LOG_TRACE("L_BUTTON is being released");
		
		if (Alexio::Input::MouseButtonPressed(Alexio::R_BUTTON))
			AIO_LOG_TRACE("R_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::R_BUTTON))
			AIO_LOG_TRACE("R_BUTTON is being released");

		if (Alexio::Input::MouseButtonPressed(Alexio::M_BUTTON))
			AIO_LOG_TRACE("M_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::M_BUTTON))
			AIO_LOG_TRACE("M_BUTTON is being released");

		return !Alexio::Input::KeyPressed(Alexio::ESCAPE);
	}
};

Alexio::Engine* Create()
{
	return new Game();
}