#include "Game.h"

Game::Game()
	:window(new Window(1200, 800))
{}

void Game::Launch()
{
	match = new LocalMatch(window, "test", 0);
	match->Start();
}
