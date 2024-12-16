#pragma once

struct MatchEnvironment;
class Actor;

// Base class for all actors' abilities
class Ability
{
private:
protected:
	bool active = false;

	MatchEnvironment* environment = nullptr;
	Actor* actor = nullptr;
public:
	Ability(MatchEnvironment* environment, Actor* actor)
		:environment(environment), actor(actor)
	{}

	virtual void Execute() {}
	virtual void Continue() {}
};