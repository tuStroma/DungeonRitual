#include "Game.h"

Game::Game()
	:window(new Window(1200, 800))
{}

void Game::Launch()
{
	match = new Match(window);
	AssetLoader::LoadMap(match, "test");
	match->Start();
}
