#pragma once
#include <iostream>
#include <vector>
#include <list>

#include "../Display/Window.h"
#include "../Display/DisplayParameters.h"
#include "../Display/Animation.h"
#include "GameObject.h"
#include "Actor.h"

#include "ObjectCollisions.h"

class Match
{
private:
	struct layer {
		Animation* animation;
		Point position;
		double depth;

		layer(Animation* animation, Point position, double depth)
			:animation(animation), position(position), depth(depth)
		{}
	};


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
	std::list<layer> background;

	void DrawObject(GameObject* obj, Uint32 color);
	void DrawSprite(SDL_Surface* sprite, Rectangle* rectangle);
	void DrawSpriteCentered(SDL_Surface* sprite, Point position, double paralax = 1);

	// Controle
	SDL_Event* event;

	void Input();
	void Update();
	void Display();

public:
	Match(Window* window);

	void addObject(GameObject* object);
	void addActor(Actor* actor, bool is_player);
	void addLayer(Animation* animation, Point position, double depth);

	void Start();
};

