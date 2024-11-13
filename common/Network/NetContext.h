#pragma once

enum NetContext
{
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
