#pragma once
#include <iostream>
#include <cmath>

#include "PhysicsParameters.h"
#include "GameObject.h"
#include "Slope.h"

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
	bool jumping = false;
	GameObject* standing_on = nullptr;
public:
	Actor();
	Actor(Rectangle* shape)
		:GameObject(shape)
	{}

	void MoveLeft(bool move)  { moving_left = move; }
	void MoveRight(bool move) { moving_right = move; }
	void Jump(bool jump);

	void Move(double t);
	void WalkOnObject(GameObject* floor, double t);

	void ResolveCollision(Point connection, GameObject* obj);

	Rectangle* getRectangle();
};

