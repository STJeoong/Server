#include <S_PacketHeader.h>
#include "LobbyServerHandler.h"
#include "UserManager.h"
#include "User.h"
#include "LobbyManager.h"
#include "Server.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <Engine.h>
#include "db_protocol.pb.h"
#include "lobby_protocol.pb.h"

#pragma region public
void LobbyServerHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial)->connect(); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial)->disconnect(); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((protocol::lobby::E_PacketID)header->id)
	{
	case protocol::lobby::E_PacketID::LOGIN_REQUEST: this->login(evt.serial, evt.data); break;
	case protocol::lobby::E_PacketID::CHAT_LOBBY_REQUEST: this->broadcastLobby(evt.serial, evt.data); break;
	default: LOG(ERROR) << "Undefined PacketID : " << header->id; break;
	}
}
#pragma endregion

#pragma region private
void LobbyServerHandler::login(int serial, char* data)
{
	User* user = UserManager::getInstance().getUser(serial);
	S_UserInfo& userInfo = user->getInfo();
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(data);
	protocol::lobby::Login_Req req;
	protocol::lobby::Login_Resp resp = {};
	protocol::db::Login_Req dbReq = {};

	if (userInfo.state != E_UserState::CONNECTED)
	{
		LOG(ERROR) << "Player(" << userInfo.name << ") is not connected. but request login";
		resp.set_resp(protocol::lobby::E_RespCode::LOGIN_INVALID_ACCESS);
		Server::getInstance().send(E_EngineType::LOBBY_SERVER, serial, { (UINT16)protocol::lobby::E_PacketID::LOGIN_RESPONSE, 0 }, resp);
		return;
	}
	req.ParseFromArray(data + sizeof(S_PacketHeader), header->initLen);
	dbReq.set_serial(serial);
	dbReq.set_id(req.id());
	dbReq.set_passwd(req.passwd());
	Server::getInstance().send(E_EngineType::DB_CLIENT, 0, {(UINT16)protocol::db::E_PacketID::LOGIN_REQUSET, 0}, dbReq);
}
void LobbyServerHandler::broadcastLobby(int serial, char* data)
{
	User* user = UserManager::getInstance().getUser(serial);
	S_UserInfo& userInfo = user->getInfo();

	if (userInfo.state != E_UserState::LOBBY && userInfo.state != E_UserState::MATCHING)
	{
		LOG(ERROR) << "Player(" << userInfo.name << ") is not in Lobby. but request broadcastLobby";
		return;
	}
	LobbyManager::getInstance().broadcast(serial, data);
}
#pragma endregion