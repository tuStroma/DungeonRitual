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

	GameObject walls[4];

	double v_x = 1;
	double v_y = 0;

	int t1 = 0, t2 = 0;

	// Camera
	Point camera = Point(0, 0);

	int posToCameraX(double x);
	int posToCameraY(double y);

	// Display
	SDL_Surface* screen;
	SDL_Renderer* renderer;
	SDL_Texture* scrtex;

	void DrawObject(GameObject* obj, SDL_Surface* surface, Uint32 color);

	// Controle
	SDL_Event* event;

	void Input();
	void Update();
	void Display();

public:
	Match(SDL_Surface* screen, SDL_Renderer* renderer, SDL_Texture* scrtex);

	void Start();
};

