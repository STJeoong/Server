#include <S_PacketHeader.h>
#include "LobbyServerHandler.h"
#include "UserManager.h"
#include "S_UserInfo.h"
#include "LobbyManager.h"
#include "E_EngineType.h"
#include "Serializer.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <Engine.h>
#include "db_protocol.pb.h"
#include "lobby_protocol.pb.h"
#include "match_protocol.pb.h"
#include "LobbyServerBroadcaster.h"

using namespace protocol;
#pragma region public
LobbyServerHandler::LobbyServerHandler()
{
	LobbyServerBroadcaster::onConnect(true, [](int serial) { printf("client(%d) connected\n", serial); });
	LobbyServerBroadcaster::onDisconnect(true, [](int serial) { printf("client(%d) disconnected\n", serial); });
	LobbyServerBroadcaster::onLoginReq(true, std::bind(&LobbyServerHandler::onLoginReq, this, std::placeholders::_1, std::placeholders::_2));
	LobbyServerBroadcaster::onChatLobbyReq(true, std::bind(&LobbyServerHandler::broadcastLobby, this, std::placeholders::_1, std::placeholders::_2));
	LobbyServerBroadcaster::onMatchReq(true, std::bind(&LobbyServerHandler::onMatchReq, this, std::placeholders::_1));
	LobbyServerBroadcaster::onMatchCancleReq(true, std::bind(&LobbyServerHandler::onMatchCancle, this, std::placeholders::_1));
}
LobbyServerHandler::~LobbyServerHandler()
{
	LobbyServerBroadcaster::onLoginReq(false, std::bind(&LobbyServerHandler::onLoginReq, this, std::placeholders::_1, std::placeholders::_2));
	LobbyServerBroadcaster::onChatLobbyReq(false, std::bind(&LobbyServerHandler::broadcastLobby, this, std::placeholders::_1, std::placeholders::_2));
	LobbyServerBroadcaster::onMatchReq(false, std::bind(&LobbyServerHandler::onMatchReq, this, std::placeholders::_1));
	LobbyServerBroadcaster::onMatchCancleReq(false, std::bind(&LobbyServerHandler::onMatchCancle, this, std::placeholders::_1));
}
#pragma endregion

#pragma region private
void LobbyServerHandler::onLoginReq(int serial, const lobby::Login_Req& req)
{
	S_UserInfo& user = UserManager::getInstance().getUser(serial);
	lobby::Login_Resp resp = {};
	db::Login_Req dbReq = {};

	if (user.state != E_UserState::CONNECTED)
	{
		LOG(ERROR) << "Player(" << user.name << ") is not connected. but request login";
		resp.set_resp(lobby::E_RespCode::LOGIN_INVALID_ACCESS);
		std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)lobby::E_PacketID::LOGIN_RESPONSE, 0 }, resp);
		Engine::send((int)E_EngineType::LOBBY_SERVER, serial, ret.first, ret.second);
		return;
	}
	printf("Login request (%s), (%s)\n", req.id().c_str(), req.passwd().c_str());
	dbReq.set_serial(serial);
	dbReq.set_id(req.id());
	dbReq.set_passwd(req.passwd());
	std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)db::E_PacketID::LOGIN_REQUSET, 0 }, dbReq);
	Engine::send((int)E_EngineType::DB_CLIENT, 0, ret.first, ret.second);
}
void LobbyServerHandler::broadcastLobby(int serial, const lobby::ChatLobby_Req& req)
{
	S_UserInfo& user = UserManager::getInstance().getUser(serial);

	if (user.state != E_UserState::LOBBY && user.state != E_UserState::MATCHING)
	{
		LOG(ERROR) << "Player(" << user.name << ") is not in Lobby. but request broadcastLobby";
		return;
	}
	LobbyManager::getInstance().broadcast(serial, req);
}
void LobbyServerHandler::onMatchReq(int serial)
{
	if (UserManager::getInstance().getUser(serial).state != E_UserState::LOBBY)
		return;
	match::Match_Req req = {};

	req.set_serial(serial);
	std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)match::E_PacketID::MATCH_REQ, 0 }, req);
	Engine::send((int)E_EngineType::MATCH_CLIENT, 0, ret.first, ret.second);
	UserManager::getInstance().getUser(serial).state = E_UserState::MATCHING;
}
void LobbyServerHandler::onMatchCancle(int serial)
{
	if (UserManager::getInstance().getUser(serial).state != E_UserState::MATCHING)
		return;
	match::MatchCancle_Req req = {};

	req.set_serial(serial);
	std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)match::E_PacketID::MATCH_CANCLE_REQ, 0 }, req);
	Engine::send((int)E_EngineType::MATCH_CLIENT, 0, ret.first, ret.second);
	UserManager::getInstance().getUser(serial).state = E_UserState::LOBBY;
}
#pragma endregion