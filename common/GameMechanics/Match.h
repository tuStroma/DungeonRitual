#pragma once
#include <iostream>
#include <vector>

#include "../Display/Window.h"
#include "../Display/DisplayParameters.h"
#include "../Display/Animation.h"
#include "GameObject.h"
#include "Actor.h"

#include "ObjectCollisions.h"

class Match
{
private:
	bool quit = false;

	// Game status
	Actor* player = nullptr;
	Animation* player_animation;
	SDL_Surface* test;

	std::vector<GameObject*> walls;
	std::vector<Slope*> slopes;

	double v_x = 1;
	double v_y = 0;

	// Time
	int t1 = 0, t2 = 0;
	int time_count = 0;
	int frame_count = 0;

	// Camera
	Point camera = Point(0, 0);

	int posToCameraX(double x);
	int posToCameraY(double y);

	// Display
	Window* window;

	void DrawObject(GameObject* obj, Uint32 color);
	void DrawSprite(SDL_Surface* sprite, Rectangle* rectangle);
	void DrawSpriteCentered(SDL_Surface* sprite, double x, double y, double paralax = 1);

	// Controle
	SDL_Event* event;

	void Input();
	void Update();
	void Display();

public:
	Match(Window* window);

	void addObject(GameObject* object);
	void addActor(Actor* actor, bool is_player);

	void Start();
};

