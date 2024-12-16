#pragma once
#include "Ability.h"

#include "../Actor.h"

class Hit : public Ability
{
private:

public:
	Hit(MatchEnvironment* environment, Actor* actor)
		:Ability(environment, actor)
	{}

	void Execute() override
	{
		//actor->Jump(true);

		for (Actor* actor : environment->actors)
		{
			double distance = geometry::Distance(actor->Position(), this->actor->Position());
			if (actor != this->actor &&
				actor->GetStandingOn() &&
				distance <= 3)
				actor->SetVerticalSpeed((3 - distance)/3 * 10);
		}

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
