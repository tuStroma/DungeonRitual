#pragma once
#include <SDL.h>

#include "../../common/GameMechanics/Match.h"

class Game
{
private:
	Match match;

public:
	Game();

	// Handles the game window and
	// every process related with game
	void Launch();

};