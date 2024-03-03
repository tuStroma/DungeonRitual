#pragma once
#include <iostream>

#include "GameObject.h"
#include "PhysicsParameters.h"

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
	Actor(Point position, double width, double height, int color);

	void MoveLeft(bool move)  { moving_left = move; }
	void MoveRight(bool move) { moving_right = move; }
	void Jump(bool jump);

	void Move(double t);

	// Colisions
	double Colision1D(double position, double previous_position, double size, double obj_a0, double obj_a1, bool& left_colision, bool& right_colision);
	void CheckColision(GameObject* obj);
};

