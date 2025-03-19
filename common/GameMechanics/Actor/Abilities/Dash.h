#pragma once
#include "Ability.h"

#include "../Actor.h"
#include "../../geometry/Collisions.h"

#define DASH_TIME 0.2
#define DASH_COOLDOWN 2

class Dash : public Ability
{
private:
	

public:
	Dash(MatchEnvironment* environment, Actor* actor)
		:Ability(environment, actor)
	{}

	void Execute() override
	{
		if (active || cooldown)
			return;

		actor->SetHorisontalSpeed(50);

		Activate();
	}

	void Continue(double delta) override
	{
		if (cooldown)
		{
			time += delta;
			if (time >= DASH_COOLDOWN)
				Reset();
			return;
		}

		if (!active)
			return;

		time += delta;

		if (time >= DASH_TIME)
		{
			Finish();
			actor->SetHorisontalSpeed(10);
		}
	}
};
