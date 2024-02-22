#include <S_PacketHeader.h>
#include "LobbyServerHandler.h"
#include "UserManager.h"
#include "S_UserInfo.h"
#include "LobbyManager.h"
#include "Server.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <Engine.h>
#include "db_protocol.pb.h"
#include "lobby_protocol.pb.h"

using namespace protocol;
#pragma region public
void LobbyServerHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial).state = E_UserState::CONNECTED; return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial).state = E_UserState::DISCONNECTED; return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((lobby::E_PacketID)header->id)
	{
	case lobby::E_PacketID::LOGIN_REQUEST: this->login(evt.serial, evt.data); break;
	case lobby::E_PacketID::CHAT_LOBBY_REQUEST: this->broadcastLobby(evt.serial, evt.data); break;
	default: LOG(ERROR) << "Undefined PacketID : " << header->id; break;
	}
}
#pragma endregion

#pragma region private
void LobbyServerHandler::login(int serial, char* data)
{
	S_UserInfo& user = UserManager::getInstance().getUser(serial);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(data);
	lobby::Login_Req req;
	lobby::Login_Resp resp = {};
	db::Login_Req dbReq = {};

	if (user.state != E_UserState::CONNECTED)
	{
		LOG(ERROR) << "Player(" << user.name << ") is not connected. but request login";
		resp.set_resp(lobby::E_RespCode::LOGIN_INVALID_ACCESS);
		Server::getInstance().send(E_EngineType::LOBBY_SERVER, serial, { (UINT16)lobby::E_PacketID::LOGIN_RESPONSE, 0 }, resp);
		return;
	}
	req.ParseFromArray(data + sizeof(S_PacketHeader), header->initLen);
	dbReq.set_serial(serial);
	dbReq.set_id(req.id());
	dbReq.set_passwd(req.passwd());
	Server::getInstance().send(E_EngineType::DB_CLIENT, 0, {(UINT16)db::E_PacketID::LOGIN_REQUSET, 0}, dbReq);
}
void LobbyServerHandler::broadcastLobby(int serial, char* data)
{
	S_UserInfo& user = UserManager::getInstance().getUser(serial);

	if (user.state != E_UserState::LOBBY && user.state != E_UserState::MATCHING)
	{
		LOG(ERROR) << "Player(" << user.name << ") is not in Lobby. but request broadcastLobby";
		return;
	}
	LobbyManager::getInstance().broadcast(serial, data);
}
#pragma endregion