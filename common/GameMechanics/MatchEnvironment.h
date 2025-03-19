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
};
