#pragma once
#include "Ability.h"

#include "../Actor.h"

class Hit : public Ability
{
private:

public:
	Hit(Actor* actor)
		:Ability(actor)
	{}

	void Execute() override
	{
		actor->Jump(true);
		active = true;
	}

	void Continue() override
	{
		if (!active)
			return;

		actor->Jump(false);
		active = false;
	}
};
