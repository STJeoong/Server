#include "Game.h"
#include <iostream>
#include "MMOServerBroadcaster.h"
#include <Engine.h>
#include "E_EngineType.h"
#include "E_TimerEvent.h"
#include "Player.h"
#include "Map.h"
#include "Monster.h"

#pragma region public
Game::Game()
{
	Map::load();
	Player::init();
	Monster::init();
	Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::UPDATE_DELTA_TIME, (int)E_TimerEvent::UPDATE);

	MMOServerBroadcaster::onConnect += [](int serial) { printf("%d connect\n", serial); };
	MMOServerBroadcaster::onDisconnect += [](int serial) { printf("%d disconnect\n", serial); };

	MMOServerBroadcaster::onUpdate += []() { Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::UPDATE_DELTA_TIME, (int)E_TimerEvent::UPDATE); };
}
#pragma endregion

#pragma region private
#pragma endregion
