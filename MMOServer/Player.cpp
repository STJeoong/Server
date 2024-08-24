#include "Player.h"
#include "MMOServerBroadcaster.h"
#include "Engine.h"
#include "E_EngineType.h"
#include "Game.h"
#include "GameObject.h"
#include "Area.h"
#include "Rectangular.h"
#include "Utils.h"
#include "Map.h"
#include "PlayerController.h"


#pragma region public static
void Player::init()
{
	MMOServerBroadcaster::onConnect += Player::onConnect;
	MMOServerBroadcaster::onDisconnect += Player::onDisconnect;
	MMOServerBroadcaster::onEnterGameReq += Player::onEnterReq;
	MMOServerBroadcaster::onMoveReq += Player::onMoveReq;

	std::cout << "Player Manager init...\n";
	s_players.resize(Engine::getEngineConfig((int)E_EngineType::MMO_SERVER).maxClient);
}
#pragma endregion

#pragma region private static
void Player::onConnect(int serial)
{
	if (s_players[serial] != nullptr) return;

	Map* mainMap = Map::getMap(MapName::MAIN);
	Player* newPlayer = mainMap->instantiate<Player>(false);
	newPlayer->state(E_ObjectState::NONE);
	s_players[serial] = newPlayer;
}
void Player::onDisconnect(int serial)
{
	Player* player = s_players[serial];
	if (player == nullptr) return;

	player->map()->destroy(player);
	s_players[serial] = nullptr;
}
void Player::onEnterReq(int serial)
{
	// TODO : 유저정보 데이터베이스에서 가져와서 조립
	EnterGame_Resp resp = {};
	Player* player = s_players[serial];
	if (player == nullptr || player->state() != E_ObjectState::NONE)
	{
		resp.set_resp(E_RespCode::NO);
		Utils::send(serial, E_PacketID::ENTER_GAME_RESP, 0, resp);
		return;
	}

	resp.set_resp(E_RespCode::OK);
	player->state(E_ObjectState::IDLE);
	player->id(serial);
	auto [baseY, baseX] = player->map()->basePoint();
	player->transform(baseY, baseX, E_Dir::BOTTOM);
	*(resp.mutable_myinfo()) = player->info();
	Area* aoi = player->addComponent<Area>();
	Area* myObjArea = player->addComponent<Area>();
	PlayerController* controller = player->addComponent<PlayerController>();
	player->_controller = controller;
	controller->myObjArea(myObjArea);

	aoi->layer(E_Layer::AOI);
	myObjArea->layer(E_Layer::PLAYER_OBJ);
	aoi->addShape(new Rectangular(aoi, -3, -3, 7, 7));
	myObjArea->addShape(new Rectangular(myObjArea, 0, 0, 1, 1));
	
	player->active(true);
	Utils::send(serial, E_PacketID::ENTER_GAME_RESP, 0, resp);
}
void Player::onMoveReq(int serial, const Move_Req& req)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE) return;
	player->_controller->move(req);
}
#pragma endregion



#pragma region public
#pragma endregion

#pragma region protected
#pragma endregion

#pragma region private
#pragma endregion

std::vector<Player*> Player::s_players;