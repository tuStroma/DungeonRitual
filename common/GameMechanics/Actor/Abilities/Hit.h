#pragma once
#include "Ability.h"

#include "../Actor.h"
#include "../../geometry/Collisions.h"

#define HIT_PREPARATION_TIME 0.1
#define HIT_TIME 0.4
#define HIT_COOLDOWN 0.5

class Hit : public Ability
{
private:
	geometry::CircularSector hit_range;

	void HitCollisions()
	{
		for (Actor* actor : environment->actors)
		{
			if (actor != this->actor &&
				geometry::collisions::SectorToRectangle(hit_range, *actor->getRectangle()))
				actor->SetVerticalSpeed(20);
		}
	}

public:
	Hit(MatchEnvironment* environment, Actor* actor)
		:Ability(environment, actor)
	{}

	void Execute() override
	{
		if (active || cooldown)
			return;

		geometry::Point centre = actor->Position();
		geometry::Point offset = geometry::Point(actor->getRectangle()->Width() / 2, 0);
		actor->isFacingRight() ? centre += offset : centre -= offset;
		double beginning = actor->isFacingRight() ? 0.125 : 0.625;
		double end = actor->isFacingRight() ? 0.375 : 0.875;

		hit_range = geometry::CircularSector(centre, 1.5, beginning, end);

		for (Actor* actor : environment->actors)
		{
			double distance = geometry::Distance(actor->Position(), this->actor->Position());
			//if (actor != this->actor &&
			//	actor->GetStandingOn() &&
			//	distance <= 3)
			//	actor->SetVerticalSpeed((3 - distance)/3 * 10);
		}

		Activate();
	}

	void Continue(double delta) override
	{
		if (cooldown)
		{
			time += delta;
			if (time >= HIT_COOLDOWN)
				Reset();
			return;
		}

		if (!active)
			return;

		time += delta;

		if (time >= HIT_PREPARATION_TIME &&
			time < HIT_TIME)
			HitCollisions();

		//actor->Jump(false);

		if (time >= HIT_PREPARATION_TIME + HIT_TIME)
			Finish();
	}
};
