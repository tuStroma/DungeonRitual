#pragma once
#include <iostream>
#include <cmath>

#include "PhysicsParameters.h"
#include "GameObject.h"
#include "Slope.h"

#include "AssetLoader/AssetLoader.h"
#include "../Display/Animation.h"

class Actor : public GameObject
{
private:
	// Actor propertes
	double speed = 10.0;
	double vertical_speed = 0.0;
	double jump_speed = 15.0;

	// Actor state
	bool moving_left = false;
	bool moving_right = false;
	bool moving_down = false;
	bool jumping = false;
	GameObject* standing_on = nullptr;

	// Animations
	Animation* idle = nullptr;

	Animation* LoadAnimation(rapidxml::xml_document<>* doc,
		std::string character,
		std::string animation,
		SDL_Renderer* renderer);

public:
	Actor();
	Actor(Rectangle* shape)
		:GameObject(shape)
	{}
	Actor(rapidxml::xml_node<>* node, std::string character, SDL_Renderer* renderer);

	void MoveLeft(bool move)  { moving_left = move; }
	void MoveRight(bool move) { moving_right = move; }
	void MoveDown(bool move)  { moving_down = move; }
	void Jump(bool jump);

	// Movement getters
	bool isAvoidingSlopes() { return moving_down; }

	void Move(double t);
	void WalkOnObject(GameObject* floor, double t);

	void ResolveCollision(Point connection, GameObject* obj);

	Rectangle* getRectangle();
	Animation* getAnimation();
};

