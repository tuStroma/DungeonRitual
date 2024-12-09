#include "Game.h"
#include "Client.h"

#define LOCAL_HOST "127.0.0.1"
#define LOCAL_NETWORK "192.168.1.77"

#define SERVER_IP LOCAL_HOST
#define SERVER_PORT 60000

void Game::OnlineGame()
{
	Client* client = new Client(window);
	client->Connect(SERVER_IP, SERVER_PORT);

	client->FindMatch();

	// Wait for beginning of the match
	while (!match)
		match = client->getMatch();

	std::cout << "Starting match\n";

	match->Start();
}

void Game::OfflineGame()
{
	match = new LocalMatch(window, "test", 1, nullptr);
	match->Start();
}

Game::Game()
	:window(new Window(1200, 800))
{}

void Game::Launch()
{
	std::cout << "Choose playing mode:\n";
	std::cout << "1. Online\n";
	std::cout << "2. Offline\n";

	std::string user_input;
	std::cin >> user_input;

	if (user_input == "1")
		OnlineGame();
	else if (user_input == "2")
		OfflineGame();
}
