#pragma once
#include <vector>
#include <map>
#include <thread>
#include <functional>
#include "../common/GameMechanics/Match.h"

class GameClient;
class Server;

class ServerMatch : public Match
{
private:
	std::map<uint64_t, GameClient*> players;
	std::map<uint64_t, OutsideController*> controllers;
	std::thread thread;

	Server* server;

	void SendGameState();

	void Input();
	void Update();

	
public:
	ServerMatch(std::string map, Server* server);

	void AddPlayer(GameClient* player);

	void StartInThread(std::function<void()> const& after_finish);
	void Start();

	void ForEachPlayer(std::function<void(uint64_t, GameClient*)> const& execute);

	// Getters
	OutsideController* getPlayerController(uint64_t client_id);
	GameClient* getGameClient(uint64_t client_id);
};

