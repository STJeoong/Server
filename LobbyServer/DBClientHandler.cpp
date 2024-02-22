#include "DBClientHandler.h"
#include "db_protocol.pb.h"
#include "lobby_protocol.pb.h"
#include "Server.h"
#include "UserManager.h"
#include "User.h"
#include "LobbyManager.h"
#include <S_PacketHeader.h>

using namespace protocol;
#pragma region public
void DBClientHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: puts("DB connected"); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: puts("DB disconnected"); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((db::E_PacketID)header->id)
	{
	case db::E_PacketID::LOGIN_RESPONSE: this->loginResp(evt); break;
	}
}
#pragma endregion

#pragma region private
void DBClientHandler::loginResp(S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	db::Login_Resp dbResp = {};
	lobby::Login_Resp lobbyResp = {};

	dbResp.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen);
	if (dbResp.resp() == db::E_RespCode::OK)
	{
		lobbyResp.set_resp(lobby::E_RespCode::OK);
		User* user = UserManager::getInstance().getUser(dbResp.serial());
		user->getInfo().name = dbResp.nickname();
		user->getInfo().state = E_UserState::LOBBY;
		LobbyManager::getInstance().inputUser(user);
	}
	else if (dbResp.resp() == db::E_RespCode::LOGIN_INVALID_PW)
		lobbyResp.set_resp(lobby::E_RespCode::LOGIN_INVALID_PW);
	else if (dbResp.resp() == db::E_RespCode::LOGIN_INVALID_ID)
		lobbyResp.set_resp(lobby::E_RespCode::LOGIN_INVALID_ID);

	Server::getInstance().send(E_EngineType::LOBBY_SERVER, dbResp.serial(), { (UINT16)lobby::E_PacketID::LOGIN_RESPONSE, 0 }, lobbyResp);
}
#pragma endregion