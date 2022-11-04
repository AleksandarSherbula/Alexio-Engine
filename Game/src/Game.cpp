#include "Game.h"
#include <iostream>

Game::Game()
{
}

void Game::OnStart()
{	
	AIO_LOG_INFO("Hello! Welcome to Alexio Game Engine");
	AIO_LOG_TRACE("Game has been traced and logged with macro");
	AIO_LOG_WARN("Game has a warning and logged with macro");
	AIO_LOG_ERROR("Game has a error and logged with macro");
	AIO_LOG_CRITICAL("Game has a critical and logged with macro");
}

void Game::OnUpdate()
{
	
}