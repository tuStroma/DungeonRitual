#pragma once
#include <SDL.h>

#include "../../common/GameMechanics/Match.h"
#include "../../common/GameMechanics/MapSetups/MapSetups.h"

class Game
{
private:
	Match* match = nullptr;

public:
	Game();

	// Handles the game window and
	// every process related with game
	void Launch();

};