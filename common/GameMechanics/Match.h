#pragma once
#include <iostream>

#include "../Display/SDL_display.h"
#include "../Display/DisplayParameters.h"
#include "GameObject.h"

class Match
{
private:
	bool quit = false;

	// Game status
	GameObject player;

	int v_x = 1;
	int v_y = 0;

	int t1 = 0, t2 = 0;

	// Camera
	int camera_x = 0;
	int camera_y = 0;

	int posToCameraX(double x);
	int posToCameraY(double y);

	// Display
	SDL_Surface* screen;
	SDL_Renderer* renderer;
	SDL_Texture* scrtex;

	void DrawObject(GameObject* obj, SDL_Surface* surface, Uint32 color);

	void Update();
	void Display();

public:
	Match(SDL_Surface* screen, SDL_Renderer* renderer, SDL_Texture* scrtex);

	void Start();
};

