#include "PlayerMoveUpdater.h"
#include "GameServerBroadcaster.h"
#include "GameManager.h"
#include "E_GameState.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Server.h"
#include "E_EngineType.h"

#pragma region public
PlayerMoveUpdater::PlayerMoveUpdater()
{
	GameServerBroadcaster::getInstance().onUpdateWorldTime(true, std::bind(&PlayerMoveUpdater::update, this));
	GameServerBroadcaster::getInstance().onMoveReq(true, std::bind(&PlayerMoveUpdater::onMoveReq, this, std::placeholders::_1, std::placeholders::_2));
}
PlayerMoveUpdater::~PlayerMoveUpdater()
{
	GameServerBroadcaster::getInstance().onUpdateWorldTime(false, std::bind(&PlayerMoveUpdater::update, this));
	GameServerBroadcaster::getInstance().onMoveReq(false, std::bind(&PlayerMoveUpdater::onMoveReq, this, std::placeholders::_1, std::placeholders::_2));
}
#pragma endregion

#pragma region private
void PlayerMoveUpdater::update()
{
	if (GameManager::gameState() != E_GameState::GAME_START)
		return;

	int serial;
	Move_Req req;
	Update_Notify notify = {};
	Player* player = nullptr;
	while (!_queue.empty())
	{
		std::tie(serial, req) = _queue.front();
		_queue.pop();
		player = PlayerManager::getInstance().getPlayer(serial);
		if (player == nullptr)
			continue;
		player->move(req);
	}

	const std::unordered_map<int, Player*>& players = PlayerManager::getInstance().getAllPlayers();
	for (const auto& val : players)
		if (val.second->hasProcessed())
			val.second->write(notify.add_players());
	if (notify.players().size() == 0)
		return;
	for (const auto& val : players)
	{
		notify.set_lpi(val.second->lpi());
		Server::getInstance().send((int)E_EngineType::GAME_SERVER, val.first, { (UINT16)E_PacketID::UPDATE_NOTIFY, 0 }, notify);
	}
}
void PlayerMoveUpdater::onMoveReq(int serial, Move_Req& req)
{
	if (GameManager::gameState() != E_GameState::GAME_START)
		return;

	printf("direction : %d\n", req.dir());
	_queue.push({ serial, req }); // TODO : req ����?
}
#pragma endregion

