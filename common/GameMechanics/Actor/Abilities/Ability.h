#pragma once

struct MatchEnvironment;
class Actor;

// Base class for all actors' abilities
class Ability
{
private:
protected:
	bool active = false;
	bool cooldown = false;
	double time = 0;

	MatchEnvironment* environment = nullptr;
	Actor* actor = nullptr;

	void Activate()
	{
		active = true;
		cooldown = false;
		time = 0;
	}

	void Finish()
	{
		active = false;
		cooldown = true;
		time = 0;
	}

	void Reset()
	{
		active = false;
		cooldown = false;
	}
public:
	Ability(MatchEnvironment* environment, Actor* actor)
		:environment(environment), actor(actor)
	{}

	virtual void Execute() {}
	virtual void Continue(double delta) {}

	bool isActive() { return active; }
};