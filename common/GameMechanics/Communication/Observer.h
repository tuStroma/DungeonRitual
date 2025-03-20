#pragma once
#include "Events/Event.h";

class Observer
{
private:
public:
	Observer() {}

	virtual void onEvent(Event* event) = 0;
};
