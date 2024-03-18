#include "GameManager.h"
#include "game_protocol.pb.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Server.h"
#include "E_EngineType.h"
#include "ThreadPool.h"
#include "GameServerBroadcaster.h"

using namespace protocol::game;
E_GameState GameManager::_state = E_GameState::READY;
#pragma region public
GameManager::GameManager()
{
	GameManager::_state = E_GameState::READY;
	GameServerBroadcaster::getInstance().onConnect(true, std::bind(&GameManager::onConnect, this, std::placeholders::_1));
	GameServerBroadcaster::getInstance().onDisconnect(true, std::bind(&GameManager::onDisconnect, this, std::placeholders::_1));
	GameServerBroadcaster::getInstance().onGameStart(true, std::bind(&GameManager::gameStart, this));
}
GameManager::~GameManager()
{
	GameManager::_state = E_GameState::GAME_OVER;
	GameServerBroadcaster::getInstance().onConnect(false, std::bind(&GameManager::onConnect, this, std::placeholders::_1));
	GameServerBroadcaster::getInstance().onDisconnect(false, std::bind(&GameManager::onDisconnect, this, std::placeholders::_1));
	GameServerBroadcaster::getInstance().onGameStart(false, std::bind(&GameManager::gameStart, this));
}
#pragma endregion

#pragma region private
void GameManager::onConnect(int serial)
{
	if (!PlayerManager::getInstance().newPlayer(serial) || PlayerManager::getInstance().size() != GameManager::GAME_START_PLAYER_COUNT)
		return;

	// TODO : 맵에 따라서 다른 초기 위치 주기
	int idx = 0;
	Welcome_Notify notify = {};
	std::pair<float, float> positions[2] = { {0,0}, {3,3} };
	const std::unordered_map<int, Player*>& players = PlayerManager::getInstance().getAllPlayers();
	for (const auto& val : players)
	{
		val.second->setPosition(positions[idx].first, positions[idx].second);
		val.second->write(notify.add_players());
		++idx;
	}
	for (const auto& val : players)
	{
		notify.set_id(val.first);
		Server::getInstance().send((int)E_EngineType::GAME_SERVER, val.first, { (UINT16)E_PacketID::WELCOME_NOTIFY, 0 }, notify);
	}
	Server::getInstance().setTimer((int)E_EngineType::GAME_SERVER, 0, GameManager::GAME_START_DELAY, E_TimerEvent::GAME_START);
}
void GameManager::onDisconnect(int serial)
{
	if (_state == E_GameState::GAME_OVER)
		return;

	GameOver_Notify notify = {};
	const std::unordered_map<int, Player*>& players = PlayerManager::getInstance().getAllPlayers();
	notify.set_iswin(true);
	for (const auto& val : players)
		if (val.first != serial)
			Server::getInstance().send((int)E_EngineType::GAME_SERVER, val.first, { (UINT16)E_PacketID::GAME_OVER_NOTIFY, 0 }, notify);
	GameManager::_state = E_GameState::GAME_OVER;
}
void GameManager::gameStart()
{
	if (GameManager::_state != E_GameState::READY)
		return;

	const std::unordered_map<int, Player*>& players = PlayerManager::getInstance().getAllPlayers();
	for (const auto& val : players)
		Server::getInstance().send((int)E_EngineType::GAME_SERVER, val.first, { (UINT16)E_PacketID::GAME_START_NOTIFY, 0 });
	GameManager::_state = E_GameState::GAME_START;
	Server::getInstance().setRepetitiveTimer((int)E_EngineType::GAME_SERVER, 0, 1000 / GameManager::UPDATE_WORLD_RATE, E_TimerEvent::UPDATE_WORLD);
}
#pragma endregion