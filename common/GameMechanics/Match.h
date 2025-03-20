#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <chrono>

#include "AssetLoader/AssetLoader.h"
#include "Objects/GameObject.h"
#include "MatchEnvironment.h"
#include "Actor/Actor.h"
#include "Actor/OutsideController.h"

#include "Actor/Abilities/Ability.h"
#include "Actor/Abilities/Hit.h"
#include "Actor/Abilities/Dash.h"

#include "Objects/ObjectCollisions.h"

#include "GameModes/GameMode.h"

class Match
{
protected:


	bool quit = false;

	// Game status

	MatchEnvironment environment;
	//std::vector<Actor*> actors;
	//std::vector<GameObject*> walls;
	//std::vector<Slope*> slopes;

	// Time
	std::chrono::system_clock::time_point t1, t2;
	std::chrono::system_clock::time_point time_count;
	int frame_count = 0;
	double time_delta = 0;

	long long TimeDelta(std::chrono::system_clock::time_point end,
						std::chrono::system_clock::time_point begin);

	void Input();
	// Returns time delta in [s]
	double UpdateTime();
	void UpdateState(double time_delta);

	Actor* LoadActor(rapidxml::xml_node<>* node, std::string character);

	// Game mode
	GameMode* game_mode;

public:
	Match(std::string map, GameMode* mode);

	void addObject(GameObject* object);
	void addActor(Actor* actor);

	Data* SerializeGameState();
	void DeserializeGameState(Data* data);

	virtual void Start();
};

