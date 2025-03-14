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
#include "PersistentChangeStats.h"
#include "Equipment.h"
#include "Consume.h"
#include "ObjectPool.h"

using namespace protocol::mmo;
#pragma region public static
void Player::init()
{
	MMOServerBroadcaster::onConnect += Player::onConnect;
	MMOServerBroadcaster::onDisconnect += Player::onDisconnect;
	MMOServerBroadcaster::onEnterGameReq += Player::onEnterReq;
	MMOServerBroadcaster::onIdleReq += Player::onIdleReq;
	MMOServerBroadcaster::onMoveReq += Player::onMoveReq;
	MMOServerBroadcaster::onNormalAttackReq += Player::onNormalAttackReq;

	std::cout << "Player Manager init...\n";
	s_players.resize(Engine::getEngineConfig((int)E_EngineType::MMO_SERVER).maxClient);
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/PlayerData.json");
	json j = json::parse(fstream);
	j.get_to(s_playerData);
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

	// TODO
	player->_stats.maxHp = 100;
	player->_stats.maxMp = 100;
	player->_stats.hp = 100;
	player->_stats.mp = 100;

	resp.set_resp(E_RespCode::OK);
	*(resp.mutable_myinfo()) = player->info();
	
	Utils::send(serial, E_PacketID::ENTER_GAME_RESP, 0, resp);
}
void Player::onMoveReq(int serial, const Move_Req& req)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE || player->state() == E_ObjectState::DEAD) return;
	player->_controller->move(req);
}
void Player::onIdleReq(int serial)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE || player->state() == E_ObjectState::DEAD) return;
	Idle_Notify notify = {};
	notify.set_id(player->id());
	player->broadcastPacket(E_PacketID::IDLE_NOTIFY, notify);
}
void Player::onNormalAttackReq(int serial)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE || player->state() == E_ObjectState::DEAD) return;
	player->_controller->normalAttack();
	NormalAttack_Notify notify = {};
	notify.set_id(player->id());
	player->broadcastPacket(E_PacketID::NORMAL_ATTACK_NOTIFY, notify);
}
void Player::onUseItem(int serial, const protocol::mmo::UseItem_Req& req)
{
	Player* player = s_players[serial];
	if (player == nullptr || player->state() == E_ObjectState::NONE || player->state() == E_ObjectState::DEAD) return;

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
bool Player::hasSpaceInEquipment() const
{
	for (int i = 0; i < Player::MAX_INVEN_SIZE; ++i)
		if (_equipments[i] == nullptr)
			return true;
	return false;
}
bool Player::hasSpaceInConsume() const
{
	for (int i = 0; i < Player::MAX_INVEN_SIZE; ++i)
		if (_consume[i] == nullptr)
			return true;
	return false;
}
void Player::getEquipment(Equipment* equipment)
{
	for (int i = 0; i < Player::MAX_INVEN_SIZE; ++i)
		if (_equipments[i] == nullptr)
		{
			_equipments[i] = equipment;
			GetEquipment_Notify notify = {};
			equipment->setNotify(notify);
			notify.set_slotidx(i);
			Utils::send(_networkSerial, E_PacketID::GET_EQUIPMENT_NOTIFY, 0, notify);
			break;
		}
}
void Player::getConsume(Consume* consume)
{
	for (int i = 0; i < Player::MAX_INVEN_SIZE; ++i)
		if (_consume[i] == nullptr)
		{
			_consume[i] = consume;
			break;
		}
}
void Player::addBuff(Buff* buff) { if (buff == nullptr) return; _buff.push_back(buff); }
void Player::removeBuff(Buff* buff)
{
	if (buff == nullptr) return;
	auto it = std::find(_buff.begin(), _buff.end(), buff);
	if (it != _buff.end())
		_buff.erase(it);
	ObjectPool::release(buff);
}
void Player::addCC(CC* cc) { if (cc == nullptr) return; _cc.push_back(cc); }
void Player::removeCC(CC* cc)
{
	if (cc == nullptr) return;
	auto it = std::find(_cc.begin(), _cc.end(), cc);
	if (it != _cc.end())
		_cc.erase(it);
	ObjectPool::release(cc);
}
void Player::addPersistentChangeStats(PersistentChangeStats* persistent) { if (persistent == nullptr) return; _persistent.push_back(persistent); }
void Player::removePersistentChangeStats(PersistentChangeStats* persistent)
{
	if (persistent == nullptr) return;
	auto it = std::find(_persistent.begin(), _persistent.end(), persistent);
	if (it != _persistent.end())
		_persistent.erase(it);
	ObjectPool::release(persistent);
}
void Player::changeStats(S_Stats delta, GameObject* who)
{
	_stats += delta;
	ChangeStats_Notify notify = {};

	notify.set_id(this->id());
	notify.set_maxhp(_stats.maxHp);
	notify.set_maxmp(_stats.maxMp);
	notify.set_atk(_stats.atk);
	notify.set_def(_stats.def);
	notify.set_speed(_stats.speed);
	notify.set_hp(_stats.hp);
	notify.set_mp(_stats.mp);

	this->broadcastPacket(E_PacketID::CHANGE_STATS_NOTIFY, notify, true);
	if (_stats.hp <= 0)
		this->state(E_ObjectState::DEAD);
}
#pragma endregion

#pragma region protected
Player::~Player()
{
	// TODO : DB에 player 상태 정보 저장

}
#pragma endregion

#pragma region private
void Player::useItem(const protocol::mmo::UseItem_Req& req)
{
	switch (req.itemtype())
	{
	case E_ItemType::EQUIPMENT:
	{
		Equipment* item = _equipments[req.slotidx()];
		if (item == nullptr) return;
		item->wear(this);
		int idx = (int)item->type();
		if (_wearing[idx] != nullptr)
		{
			_wearing[idx]->unwear(this);
			_equipments[req.slotidx()] = _wearing[idx];
		}
		_equipments[req.slotidx()] = nullptr;
		_wearing[idx] = item;
		break;
	}
	case E_ItemType::CONSUME:
	{
		Consume* item = _consume[req.slotidx()];
		if (item == nullptr) return;
		// TODO
		break;
	}
	}
}
#pragma endregion

std::vector<Player*> Player::s_players;
std::vector<S_PlayerData> Player::s_playerData;