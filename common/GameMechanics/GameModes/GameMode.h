#pragma once
#include "../Communication/Observer.h"
#include "../MatchEnvironment.h"

class GameMode : public Observer
{
private:
	MatchEnvironment* environment = nullptr;
public:
	GameMode()
	{}

	void SetEnvironment(MatchEnvironment* environment)
	{
		this->environment = environment;
	}
};
