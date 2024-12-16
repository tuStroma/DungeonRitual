#pragma once

class Actor;

// Base class for all actors' abilities
class Ability
{
private:
protected:
	bool active = false;
	Actor* actor = nullptr;
public:
	Ability(Actor* actor) 
		:actor(actor)
	{}

	virtual void Execute() {}
	virtual void Continue() {}
};