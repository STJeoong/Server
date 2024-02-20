#include "DBClientHandler.h"
#include "db_protocol.pb.h"
#include "lobby_protocol.pb.h"
#include "Server.h"
#include "UserManager.h"
#include "User.h"
#include "LobbyManager.h"
#include <S_PacketHeader.h>

#pragma region public
void DBClientHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: puts("DB connected"); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: puts("DB disconnected"); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((protocol::db::E_PacketID)header->id)
	{
	case protocol::db::E_PacketID::LOGIN_RESPONSE: this->loginResp(evt); break;
	}
}
#pragma endregion

#pragma region private
void DBClientHandler::loginResp(S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	protocol::db::Login_Resp dbResp = {};
	protocol::lobby::Login_Resp lobbyResp = {};
	S_PacketAttr attr = {};

	dbResp.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen);
	if (dbResp.resp() == protocol::db::E_RespCode::OK)
	{
		lobbyResp.set_resp(protocol::lobby::E_RespCode::OK);
		User* user = UserManager::getInstance().getUser(dbResp.serial());
		user->getInfo().name = dbResp.nickname();
		user->getInfo().state = E_UserState::LOBBY;
		LobbyManager::getInstance().inputUser(user);
	}
	else if (dbResp.resp() == protocol::db::E_RespCode::LOGIN_INVALID_PW)
		lobbyResp.set_resp(protocol::lobby::E_RespCode::LOGIN_INVALID_PW);
	attr.packetID = (UINT16)protocol::lobby::E_PacketID::LOGIN_RESPONSE;
	attr.option = 0;

	Server::getInstance().send(E_EngineType::LOBBY_SERVER, dbResp.serial(), attr, lobbyResp);
}
#pragma endregion