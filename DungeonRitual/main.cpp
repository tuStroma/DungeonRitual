#include <iostream>

#include "Game/Game.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello\n";

	Game game = Game();
	game.Launch();

	return 0;
}