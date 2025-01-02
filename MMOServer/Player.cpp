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
#include "S_RectDefine.h"
#include "Buff.h"
#include "CC.h"	
#include "PersistentHit.h"

using namespace protocol::mmo;
#pragma region public static
void Player::init()
{
	MMOServerBroadcaster::onConnect += Player::onConnect;
	MMOServerBroadcaster::onDisconnect += Player::onDisconnect;
	MMOServerBroadcaster::onEnterGameReq += Player::onEnterReq;
	MMOServerBroadcaster::onIdleReq += Player::onIdleReq;
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
	PlayerController* controller = newPlayer->addComponent<PlayerController>();
	Area* aoi = newPlayer->addComponent<Area>();
	Area* myObjArea = newPlayer->addComponent<Area>();

	newPlayer->state(E_ObjectState::NONE);
	newPlayer->id(Utils::createID(E_ObjectType::PLAYER, 0, serial));
	newPlayer->_controller = controller;
	newPlayer->_objArea = myObjArea;
	controller->myObjArea(myObjArea);

	S_RectDefine aoiDef = {-10, -10, 21, 21};
	S_RectDefine objDef = { 0,0,1,1 };
	aoi->layer(E_Layer::AOI);
	myObjArea->layer(E_Layer::PLAYER_OBJ);
	aoi->addShape(new Rectangular(aoi, aoiDef));
	myObjArea->addShape(new Rectangular(myObjArea, objDef));

	s_players[serial] = newPlayer;
	newPlayer->_networkSerial = serial;
}
void Player::onDisconnect(int serial)
{
	Player* player = s_players[serial];
	if (player == nullptr) return;

	player->map()->destroy(player);
	player->active(false);
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

	player->state(E_ObjectState::IDLE);
	auto [baseY, baseX] = player->map()->basePoint();
	player->transform(baseY, baseX, (E_Dir)0);
	player->active(true);
	player->flipX(false);

	resp.set_resp(E_RespCode::OK);
	*(resp.mutable_myinfo()) = player->info();
	
	Utils::send(serial, E_PacketID::ENTER_GAME_RESP, 0, resp);
}
void Player::onMoveReq(int serial, const Move_Req& req)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE) return;
	player->_controller->move(req);
}
void Player::onIdleReq(int serial)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE) return;
	Idle_Notify notify = {};
	notify.set_id(player->id());
	player->broadcastPacket(E_PacketID::IDLE_NOTIFY, notify);
}
#pragma endregion



#pragma region public
void Player::broadcastPacket(protocol::mmo::E_PacketID packetID , bool includeMe)
{
	for (Area* area : _objArea->overlappedAreas())
		if (area->layer() == E_Layer::AOI)
		{
			Player* otherPlayer = reinterpret_cast<Player*>(area->gameObject());
			Utils::send(otherPlayer->_networkSerial, packetID, 0);
		}
	if (includeMe)
		Utils::send(_networkSerial, packetID, 0);
}
void Player::broadcastPacket(protocol::mmo::E_PacketID packetID, google::protobuf::Message& message, bool includeMe)
{
	for (Area* area : _objArea->overlappedAreas())
		if (area->layer() == E_Layer::AOI)
		{
			Player* otherPlayer = reinterpret_cast<Player*>(area->gameObject());
			Utils::send(otherPlayer->_networkSerial, packetID, 0, message);
		}
	if (includeMe)
		Utils::send(_networkSerial, packetID, 0, message);
}
void Player::addBuff(Buff* buff) { _buff.push_back(buff); }
void Player::removeBuff(Buff* buff)
{
	auto it = std::find(_buff.begin(), _buff.end(), buff);
	if (it != _buff.end())
		_buff.erase(it);
}
void Player::addCC(CC* cc) { _cc.push_back(cc); }
void Player::removeCC(CC* cc)
{
	auto it = std::find(_cc.begin(), _cc.end(), cc);
	if (it != _cc.end())
		_cc.erase(it);
}
void Player::addPersistentHit(PersistentHit* persistentHit) { _persistentHit.push_back(persistentHit); }
void Player::removePersistentHit(PersistentHit* persistentHit)
{
	auto it = std::find(_persistentHit.begin(), _persistentHit.end(), persistentHit);
	if (it != _persistentHit.end())
		_persistentHit.erase(it);
}
void Player::takeDamage(protocol::mmo::E_Stats what, int val)
{
	// TODO
}
#pragma endregion

#pragma region protected
#pragma endregion

#pragma region private
#pragma endregion

std::vector<Player*> Player::s_players;