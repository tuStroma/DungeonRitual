#pragma once
#include <iostream>
#include <vector>
#include <list>

#include "../Display/Window.h"
#include "../Display/DisplayParameters.h"
#include "../Display/Animation.h"
#include "../Display/SurfacePainter.h"
#include "AssetLoader/AssetLoader.h"
#include "GameObject.h"
#include "Actor/Actor.h"
#include "Actor/OutsideController.h"

#include "ObjectCollisions.h"

class Match
{
protected:


	bool quit = false;

	// Game status
	int player_index = 0;
	OutsideController* player_controller = nullptr;

	std::vector<Actor*> actors;
	std::vector<GameObject*> walls;
	std::vector<Slope*> slopes;

	// Time
	int t1 = 0, t2 = 0;
	int time_count = 0;
	int frame_count = 0;

	// Controle
	SDL_Event* event;

	void Input();
	void Update();

	Actor* LoadActor(rapidxml::xml_node<>* node, std::string character);

public:
	Match(std::string map, int player_index);

	void addObject(GameObject* object);
	void addActor(Actor* actor);

	virtual void Start();
};

