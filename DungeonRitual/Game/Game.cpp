#include "Game.h"
#include "Client.h"

#define LOCAL_HOST "127.0.0.1"
#define LOCAL_NETWORK "192.168.1.77"

#define SERVER_IP LOCAL_HOST
#define SERVER_PORT 60000

Game::Game()
	:window(new Window(1200, 800))
{}

void Game::Launch()
{
	//match = new LocalMatch(window, "test", 1);
	//match->Start();

	Client* client = new Client(window);
	client->Connect(SERVER_IP, SERVER_PORT);

	client->FindMatch();

	// Wait for beginning of the match
	//int player = -1;
	//while (player < 0)
	//	player = client->getPosition();

	while (!match)
		match = client->getMatch();

	//std::cout << "Starting match [" << position << "\n";
	//match = new LocalMatch(window, "test", 0);

	std::cout << "Starting match\n";

	match->Start();
}
