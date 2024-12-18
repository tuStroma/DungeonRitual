#pragma once
#include <iostream>
#include <cmath>

#include "Abilities/Ability.h"

#include "../PhysicsParameters.h"
#include "../GameObject.h"
#include "../Slope.h"

#include "../AssetLoader/AssetLoader.h"

#include "../../../common/Data.h"

class ActorController;
class OutsideController;

class Actor : public GameObject
{
private:
	// Actor propertes
	double speed = 10.0;
	double vertical_speed = 0.0;
	double jump_speed = 15.0;

	// Actor state
	bool facing_right = true;

	bool moving_left = false;
	bool moving_right = false;
	bool moving_down = false;
	bool jumping = false;
	GameObject* standing_on = nullptr;

	ActorController* controller = nullptr;

	Ability* basic_attack = nullptr;

	void CheckFacingDirection();

	void Move(double t);
	void WalkOnObject(GameObject* floor, double t);

	void TakeAction();

public:
	Actor();
	Actor(geometry::Rectangle* shape)
		:GameObject(shape)
	{}
	Actor(rapidxml::xml_node<>* node);

	void AddAbility(Ability* ability);
	Ability* ActiveAbility();

	// Actor controlls
	void MoveLeft(bool move)  { moving_left = move; }
	void MoveRight(bool move) { moving_right = move; }
	void MoveDown(bool move)  { moving_down = move; }
	void Jump(bool jump);

	void BasicAttack();

	bool isFacingRight() { return facing_right; }
	ActorController* getController() { return controller; }

	// Movement getters
	bool isAvoidingSlopes() { return moving_down; }
	bool isMoving();

	// State
	GameObject* GetStandingOn() { return standing_on; }
	void SetVerticalSpeed(double speed);

	void Update(double t);

	void ResolveCollision(geometry::Point connection, GameObject* obj);

	geometry::Rectangle* getRectangle();

	// Serialization
	Data* Serialize();
	static size_t SerializationSize();

	void Deserialize(Data* data);
};

