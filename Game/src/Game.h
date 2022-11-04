#ifndef GAME_H
#define GAME_H

#include "AlexioEngine.h"

class Game : public Alexio::Engine
{
public:
	void OnStart() override;
	void OnUpdate() override;
};

#endif // !GAME_H


