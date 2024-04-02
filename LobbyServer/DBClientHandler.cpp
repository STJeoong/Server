#include "DBClientHandler.h"
#include "lobby_protocol.pb.h"
#include "UserManager.h"
#include "LobbyManager.h"
#include "E_EngineType.h"
#include <S_PacketHeader.h>
#include "DBClientBroadcaster.h"
#include "Serializer.h"
#include <Engine.h>

using namespace protocol;
#pragma region public
DBClientHandler::DBClientHandler()
{
	DBClientBroadcaster::onConnect(true, [](int serial) { printf("db connected\n"); });
	DBClientBroadcaster::onDisconnect(true, [](int serial) { printf("db disconnected\n"); });
	DBClientBroadcaster::onLoginResp(true, std::bind(&DBClientHandler::onLoginResp, this, std::placeholders::_1));
}
DBClientHandler::~DBClientHandler()
{
	DBClientBroadcaster::onLoginResp(false, std::bind(&DBClientHandler::onLoginResp, this, std::placeholders::_1));
}
#pragma endregion

#pragma region private
void DBClientHandler::onLoginResp(const db::Login_Resp& resp)
{
	lobby::Login_Resp lobbyResp = {};

	if (resp.resp() == db::E_RespCode::OK)
	{
		lobbyResp.set_resp(lobby::E_RespCode::OK);
		S_UserInfo& user = UserManager::getInstance().getUser(resp.serial());
		user.name = resp.nickname();
		user.state = E_UserState::LOBBY;
		LobbyManager::getInstance().inputUser(&user);
	}
	else if (resp.resp() == db::E_RespCode::LOGIN_INVALID_PW)
		lobbyResp.set_resp(lobby::E_RespCode::LOGIN_INVALID_PW);
	else if (resp.resp() == db::E_RespCode::LOGIN_INVALID_ID)
		lobbyResp.set_resp(lobby::E_RespCode::LOGIN_INVALID_ID);

	std::pair<Size, char*> ret = Serializer::serialize({(UINT16)lobby::E_PacketID::LOGIN_RESPONSE, 0}, lobbyResp);
	Engine::send((int)E_EngineType::LOBBY_SERVER, resp.serial(), ret.first, ret.second);
}
#pragma endregion