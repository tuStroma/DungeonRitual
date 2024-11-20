#include "ServerMatch.h"
#include "GameClient.h"

void ServerMatch::Input()
{
	for (auto& [player_id, controller] : controllers)
	{
		int player_number = players[player_id]->getMatchId();
		Actor* player = actors[player_number];
		controller->TakeAction(player);
	}
}

void ServerMatch::Update()
{
	double time_delta = UpdateTime();

	// Print frames
	if (TimeDelta(t2, time_count) >= 1000000)
	{
		std::cout << frame_count << "\n";
		frame_count = 0;
		time_count = t2;
	}

	UpdateState(time_delta);
}

ServerMatch::ServerMatch(std::string map)
	:Match(map)
{}

void ServerMatch::AddPlayer(GameClient * player)
{
	players[player->getClientId()] = player;
	controllers[player->getClientId()] = new OutsideController();
}

void ServerMatch::StartInThread(std::function<void()> const& after_finish)
{
	thread = std::thread([&]() {
		Start();
		after_finish();
		});
}

void ServerMatch::Start()
{
	t1 = std::chrono::system_clock::now();

	while (!quit)
	{
		// Read input
		Input();

		// Update game state
		Update();
	}
}

void ServerMatch::ForEachPlayer(std::function<void(uint64_t, GameClient*)> const& execute)
{
	for (auto& [player_id, game_client] : players)
		execute(player_id, game_client);
}

OutsideController* ServerMatch::getPlayerController(uint64_t client_id)
{
	return controllers[client_id];
}

GameClient* ServerMatch::getGameClient(uint64_t client_id)
{
	return players[client_id];
}