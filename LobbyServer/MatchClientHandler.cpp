#include "MatchClientHandler.h"
#include "lobby_protocol.pb.h"
#include "UserManager.h"
#include <S_PacketHeader.h>
#include <Engine.h>
#include "MatchClientBroadcaster.h"
#include "Serializer.h"
#include "E_EngineType.h"

using namespace protocol;

#pragma region public
MatchClientHandler::MatchClientHandler()
{
	MatchClientBroadcaster::onConnect(true, [](int serial) { printf("match server connected\n"); });
	MatchClientBroadcaster::onDisconnect(true, [](int serial) { printf("match server disconnected\n"); });
	MatchClientBroadcaster::onMatchResp(true, std::bind(&MatchClientHandler::onMatchResp, this, std::placeholders::_1));
}
MatchClientHandler::~MatchClientHandler()
{
	MatchClientBroadcaster::onMatchResp(false, std::bind(&MatchClientHandler::onMatchResp, this, std::placeholders::_1));
}
#pragma endregion

#pragma region private
void MatchClientHandler::onMatchResp(const protocol::match::Match_Resp& resp)
{
	lobby::Match_Resp lobbyResp = {};

	lobbyResp.set_ip(resp.ip());
	lobbyResp.set_port(resp.port());

	auto serials = resp.serials();
	std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)lobby::E_PacketID::MATCH_RESP, 0 }, lobbyResp);
	for (int i = 0; i < serials.size(); ++i)
	{
		Engine::send((int)E_EngineType::LOBBY_SERVER, serials[i], ret.first, ret.second);
		UserManager::getInstance().getUser(serials[i]).state = E_UserState::INGAME;
	}
}
#pragma endregion