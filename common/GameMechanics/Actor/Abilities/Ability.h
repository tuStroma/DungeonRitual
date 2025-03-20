#pragma once
#include <list>
#include "../../Communication/Observer.h"

struct MatchEnvironment;
class Actor;

// Base class for all actors' abilities
class Ability
{
private:
	double preparation_time = 0;
	double ability_time = 0;
	double cooldown_time = 0;

	// Ability status
	bool active = false;
	bool cooldown = false;
	double time = 0;

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

	// Observer list
	std::list<Observer*> observers;

protected:
	// Environment references
	MatchEnvironment* environment = nullptr;
	Actor* actor = nullptr;

	// Communication
	void NotifyObservers(Event* event)
	{
		for (Observer* observer : observers)
		{
			observer->onEvent(event);
		}
	}

	// Action functions to override in child classes
	virtual void ExecuteAbility() {}
	virtual void ContinueAbility(double delta) {}
	virtual void FinalizeAbility() {}
public:

	// Setup
	Ability(MatchEnvironment* environment, Actor* actor, double preparation_t, double ability_t, double cooldown_t)
		:environment(environment),
		actor(actor),
		preparation_time(preparation_t),
		ability_time(ability_t),
		cooldown_time(cooldown_t)
	{}

	void addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	// Ability status
	bool isActive() { return active; }

	// Performing ablity
	void Execute()
	{
		if (active || cooldown)
			return;

		ExecuteAbility();

		Activate();
	}

	void Continue(double delta)
	{
		if (cooldown)
		{
			time += delta;
			if (time >= cooldown_time)
				Reset();
			return;
		}

		if (!active)
			return;

		time += delta;

		if (time >= preparation_time &&
			time < ability_time)
			ContinueAbility(delta);

		if (time >= preparation_time + ability_time)
		{
			FinalizeAbility();
			Finish();
		}
	}
};