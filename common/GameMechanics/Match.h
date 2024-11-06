#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <chrono>

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
	std::chrono::system_clock::time_point t1, t2;
	std::chrono::system_clock::time_point time_count;
	int frame_count = 0;

	long long TimeDelta(std::chrono::system_clock::time_point end,
						std::chrono::system_clock::time_point begin);

	void Input();
	void Update();

	Actor* LoadActor(rapidxml::xml_node<>* node, std::string character);

public:
	Match(std::string map, int player_index);

	void addObject(GameObject* object);
	void addActor(Actor* actor);

	virtual void Start();
};

