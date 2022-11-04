#include "Game.h"
#include <iostream>

void Game::OnStart()
{	
	ALEXIO_LOG_INFO("Hello! Welcome to Alexio Game Engine");
	ALEXIO_LOG_TRACE("Game has been traced and logged with macro");
	ALEXIO_LOG_WARN("Game has a warning and logged with macro");
	ALEXIO_LOG_ERROR("Game has a error and logged with macro");
	ALEXIO_LOG_CRITICAL("Game has a critical and logged with macro");

}

void Game::OnUpdate()
{
	
}