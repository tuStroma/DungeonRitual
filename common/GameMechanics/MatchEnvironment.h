#pragma once
#include <vector>

#include "Objects/GameObject.h"
#include "Objects/Slope.h"
#include "Actor/Actor.h"

struct MatchEnvironment
{
	std::vector<Actor*> actors;
	std::vector<GameObject*> walls;
	std::vector<Slope*> slopes;

	int FindActorIndex(Actor* actor)
	{
		auto iterator = std::find(actors.begin(), actors.end(), actor);

		if (iterator == actors.end())
			return -1;

		return std::distance(actors.begin(), iterator);
	}
};
