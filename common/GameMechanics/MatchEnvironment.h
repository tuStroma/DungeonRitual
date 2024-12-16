#pragma once
#include <vector>

#include "GameObject.h"
#include "Slope.h"
#include "Actor/Actor.h"

struct MatchEnvironment
{
	std::vector<Actor*> actors;
	std::vector<GameObject*> walls;
	std::vector<Slope*> slopes;
};
