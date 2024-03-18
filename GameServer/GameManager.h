#pragma once
#include <vector>
#include "E_GameState.h"
class GameManager
{
	static const int GAME_START_PLAYER_COUNT = 2;
	static const int GAME_START_DELAY = 2000;
	static const int UPDATE_WORLD_RATE = 30;
public:
	static E_GameState gameState() { return _state; }
	GameManager();
	~GameManager();
private:
	void onConnect(int serial);
	void onDisconnect(int serial);
	void gameStart();

	static E_GameState _state;
};

