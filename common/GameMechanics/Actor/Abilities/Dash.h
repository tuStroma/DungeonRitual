#pragma once
#include "Ability.h"

#include "../Actor.h"
#include "../../Objects/geometry/Collisions.h"

#define DASH_TIME 0.2
#define DASH_COOLDOWN 2

class Dash : public Ability
{
private:
	

public:
	Dash(MatchEnvironment* environment, Actor* actor)
		:Ability(environment, actor, 0, DASH_TIME, DASH_COOLDOWN)
	{}

	void ExecuteAbility() override
	{
		actor->SetHorisontalSpeed(50);
	}

	void ContinueAbility(double delta) override
	{
	}

	void FinalizeAbility() override
	{
		actor->SetHorisontalSpeed(10);
	}
};
