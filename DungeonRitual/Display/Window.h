#pragma once
#include <SDL.h>

class Window
{
private:
	// Window properties
	int screen_width;
	int screen_height;

	// SDL
	SDL_Window* window;
	SDL_Renderer* renderer;


	int CentreToWindowPosition_X(int x)
	{
		return (int)(screen_width / 2 + x);
	}
	int CentreToWindowPosition_Y(int y)
	{
		return (int)(screen_height / 2 - y);
	}

public:
	Window(int width, int height)
		:screen_width(width), screen_height(height)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow("Dungeon Ritual", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	// Draw
	void DrawSprite(SDL_Texture* sprite, int x, int y, int width, int height, double rotation = 0, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		int begin_x = x - width / 2;
		int begin_y = y + height / 2;

		SDL_Rect dest;
		dest.x = CentreToWindowPosition_X(begin_x);
		dest.y = CentreToWindowPosition_Y(begin_y);
		dest.w = width;
		dest.h = height;

		SDL_RenderCopyEx(renderer, sprite, NULL, &dest, rotation, NULL, flip);
	}

	void DisplayFrame()
	{
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	SDL_Renderer* GetRenderer()
	{
		return renderer;
	}
};
