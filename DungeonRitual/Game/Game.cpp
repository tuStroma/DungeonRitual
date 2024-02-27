#include "Game.h"

Game::Game()
{}

void Game::Launch()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Dungeon Ritual", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Surface* screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	SDL_Texture* scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);

	match = new Match(screen, renderer, scrtex);
	match->Start();
}
