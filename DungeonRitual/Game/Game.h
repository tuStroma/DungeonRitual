#pragma once
#include <SDL.h>

#include "LocalMatch.h"

#include "../Display/Window.h"
#include "../../common/GameMechanics/Match.h"
#include "../../common/GameMechanics/AssetLoader/AssetLoader.h"

class Game
{
private:
	Window* window = nullptr;

	Match* match = nullptr;

	void OnlineGame();
	void OfflineGame();

public:
	Game();

	// Handles the game window and
	// every process related with game
	void Launch();

};