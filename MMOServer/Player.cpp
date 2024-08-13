#include "Player.h"
#include "MMOServerBroadcaster.h"
#include <Engine.h>
#include "E_EngineType.h"
#include "Utils.h"

#pragma region public
void Player::init()
{
	const S_ServerConfig& config = Engine::getEngineConfig((int)E_EngineType::MMO_SERVER);
	s_maxPlayer = config.maxClient;
	s_players = new Player[config.maxClient];

	// register events
	MMOServerBroadcaster::onEnterGameReq += Player::onPlayerEnter;
	MMOServerBroadcaster::onDisconnect += Player::onPlayerExit;
}
void Player::broadcast(UINT16 packetID, UINT8 option, const google::protobuf::Message& message)
{
	for (int i = 0; i < s_maxPlayer; ++i)
		if (s_players[i]._info.state() != E_PlayerState::NONE)
			Utils::send((int)E_EngineType::MMO_SERVER, s_players[i]._info.id(), packetID, option, message);
}
void Player::broadcast(UINT16 packetID, UINT8 option)
{
	for (int i = 0; i < s_maxPlayer; ++i)
		if (s_players[i]._info.state() != E_PlayerState::NONE)
			Utils::send((int)E_EngineType::MMO_SERVER, s_players[i]._info.id(), packetID, option);
}
void Player::writeAllPlayerInfo(google::protobuf::RepeatedPtrField<PlayerInfo>* infos)
{
	for (int i = 0; i < s_maxPlayer; ++i)
		if (s_players[i]._info.state() != E_PlayerState::NONE)
		{
			PlayerInfo* info = infos->Add();
			*info = s_players[i]._info;
		}
}
#pragma endregion

#pragma region private
void Player::onPlayerEnter(int serial)
{
	Player& player = s_players[serial];
	if (player._info.state() != E_PlayerState::NONE)
	{
		EnterGame_Resp packet = {};
		packet.set_resp(E_RespCode::NO);
		Utils::send((int)E_EngineType::MMO_SERVER, serial, (UINT16)E_PacketID::ENTER_GAME_RESP, 0, packet);
		return;
	}
	// TODO : enter
	player.spawn(serial, 0, 0);
	EnterGame_Resp enterGameResp = {};
	enterGameResp.set_resp(E_RespCode::OK);
	enterGameResp.set_id(serial);
	auto players = enterGameResp.mutable_players();
	Player::writeAllPlayerInfo(players);

	PlayerEnter_Notify enterNotify = {};
	PlayerInfo* info = enterNotify.mutable_info();
	*info = player._info;
	
	Utils::send((int)E_EngineType::MMO_SERVER, serial, (UINT16)E_PacketID::ENTER_GAME_RESP, 0, enterGameResp);
	Player::broadcast((UINT16)E_PacketID::PLAYER_ENTER_NOTIFY, 0, enterNotify);
}
void Player::onPlayerExit(int serial)
{
	Player& player = s_players[serial];
	if (player._info.state() == E_PlayerState::NONE)
		return;
	// TODO : exit
	PlayerExit_Notify notify = {};
	notify.set_id(serial);
	player.despawn();
	Player::broadcast((UINT16)E_PacketID::PLAYER_EXIT_NOTIFY, 0, notify);
}


void Player::spawn(int id, int x, int y)
{
	_info.set_state(E_PlayerState::IDLE);
	Vec2* pos = _info.mutable_pos();
	_info.set_id(id);
	pos->set_x(x);
	pos->set_y(y);
}
void Player::despawn()
{
	_info.set_state(E_PlayerState::NONE);
}
#pragma endregion

int Player::s_maxPlayer = 0;
Player* Player::s_players = nullptr;