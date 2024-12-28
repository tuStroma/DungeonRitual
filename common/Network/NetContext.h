#pragma once

enum NetContext
{
	// Ping
	ServerPing,
	ClientPing,

	// Match handling
	FindGame,
	GameFound,
	PlayerReady,
	GameStart,
	GameFinished,
	LeaveGame,

	// Match communication
	MoveAction,
	GameState
};
