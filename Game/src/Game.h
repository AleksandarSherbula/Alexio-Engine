#ifndef GAME_H
#define GAME_H

#include "Alexio.h"

class Game : public Alexio::Engine
{
public:
	Game();
	void OnStart() override;
	void OnUpdate() override;
};

#endif // !GAME_H


