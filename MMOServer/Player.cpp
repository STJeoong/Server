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

#pragma region public static
void Player::init()
{
	MMOServerBroadcaster::onConnect += Player::onConnect;
	MMOServerBroadcaster::onDisconnect += Player::onDisconnect;
	MMOServerBroadcaster::onEnterGameReq += Player::onEnterReq;
	MMOServerBroadcaster::onMoveReq += Player::onMoveReq;

	std::cout << "Player Manager init...\n";
	s_players.resize(Engine::getEngineConfig((int)E_EngineType::MMO_SERVER).maxClient);
	for (int i = 0; i < s_players.size(); ++i)
		s_players[i] = Player::createPlayer();
}
#pragma endregion

#pragma region private static
Player* Player::createPlayer()
{
	GameObject* obj = Game::instantiate(false);
	Player* ret = obj->addComponent<Player>();
	Area* aoi = obj->addComponent<Area>();
	Area* objArea = obj->addComponent<Area>();

	aoi->layer(E_Layer::AOI);
	aoi->addShape(new Rectangular(0, 0, 5, 5));
	objArea->layer(E_Layer::PLAYER_OBJ);
	// TODO : 크기가 1이 아님. 2x2가 됨.
	objArea->addShape(new Rectangular(0, 0, 1, 1));

	obj->id(GameObject::INVALID_ID);
	obj->state(E_ObjectState::NONE);
	obj->transform(2, 2, E_Dir::BOTTOM);
	ret->_aoi = aoi;
	ret->_myObjArea = objArea;
	return ret;
}
void Player::onConnect(int serial)
{
	GameObject* obj = s_players[serial]->gameObject();
	// 이미 연결돼있음
	if (obj->id() != GameObject::INVALID_ID) return;
	obj->id(serial);
	obj->state(E_ObjectState::NONE);
}
void Player::onDisconnect(int serial)
{
	Player* player = s_players[serial];
	GameObject* obj = player->gameObject();
	// 이미 연결종료돼있음.
	if (obj->id() == GameObject::INVALID_ID) return;
	if (obj->activeSelf())
		obj->active(false);
	obj->id(GameObject::INVALID_ID);
	obj->state(E_ObjectState::NONE);
	player->_map = nullptr;
}
void Player::onEnterReq(int serial)
{
	Player* player = s_players[serial];
	GameObject* obj = player->gameObject();
	EnterGame_Resp resp = {};
	// 연결이 안돼있거나 이미 게임에 입장했으면 no
	if (obj->id() == GameObject::INVALID_ID || obj->state() != E_ObjectState::NONE)
	{
		resp.set_resp(E_RespCode::NO);
		Utils::send(serial, E_PacketID::ENTER_GAME_RESP, 0, resp);
		return;
	}
	obj->active(true);
	obj->transform(2, 2, E_Dir::BOTTOM);
	obj->state(E_ObjectState::IDLE);
	player->_map = Map::getMap(MapName::MAIN);

	resp.set_resp(E_RespCode::OK);
	*(resp.mutable_myinfo()) = obj->info();
	Utils::send(serial, E_PacketID::ENTER_GAME_RESP, 0, resp);
}
void Player::onMoveReq(int serial, const Move_Req& req)
{
	Player* player = s_players[serial];
	GameObject* obj = player->gameObject();
	if (obj->id() == GameObject::INVALID_ID || obj->state() == E_ObjectState::NONE)
		return;
	player->move(req);
}
#pragma endregion



#pragma region public
#pragma endregion

#pragma region protected
void Player::onAreaEnter(Area& my, Area& other)
{
	if (my.layer() != E_Layer::AOI) return;

	// spawn
	ObjectEnter_Notify notify = {};
	*(notify.mutable_otherinfo()) = other.gameObject()->info();
	Utils::send(_gameObject->id(), E_PacketID::OBJECT_ENTER_NOTIFY, 0, notify);
}
void Player::onAreaExit(Area& my, Area& other)
{
	// 내가 사라진거거나 AOI가 아니면 return
	if (!_gameObject->activeInHierarchy() || my.layer() != E_Layer::AOI) return;

	// despawn
	ObjectExit_Notify notify = {};
	notify.set_otherid(other.gameObject()->id());
	Utils::send(_gameObject->id(), E_PacketID::OBJECT_EXIT_NOTIFY, 0, notify);
}
#pragma endregion

#pragma region private
void Player::move(const Move_Req& req)
{
	static int dy[4] = { 1,0,-1,0 };
	static int dx[4] = { 0,-1,0,1 };
	const TransformInt& worldTF = _gameObject->transform();
	
	if (!_map->canGo(worldTF.y() + dy[(int)req.dir()], worldTF.x() + dx[(int)req.dir()]))
		return;
	_gameObject->transform(worldTF.y() + dy[(int)req.dir()], worldTF.x() + dx[(int)req.dir()], req.dir());
	_gameObject->state(E_ObjectState::MOVE);
	// 나를 볼 수 있는 플레이어들에게 알려주기
	Move_Notify notify = {};
	notify.set_dir(req.dir());
	notify.set_id(_gameObject->id());
	for (Area* area : _myObjArea->overlappedAreas())
		if (area->layer() == E_Layer::AOI)
			Utils::send(area->gameObject()->id(), E_PacketID::MOVE_NOTIFY, 0, notify);
	// 본인에게도 알려주기
	Utils::send(_gameObject->id(), E_PacketID::MOVE_NOTIFY, 0, notify);
}
#pragma endregion

std::vector<Player*> Player::s_players;