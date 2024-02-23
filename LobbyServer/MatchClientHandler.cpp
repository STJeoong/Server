#include "MatchClientHandler.h"
#include "match_protocol.pb.h"
#include "lobby_protocol.pb.h"
#include "UserManager.h"
#include "Server.h"
#include <S_PacketHeader.h>

using namespace protocol;

#pragma region public
void MatchClientHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: puts("match server connected"); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: puts("match server disconnected"); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((match::E_PacketID)header->id)
	{
	case match::E_PacketID::MATCH_RESP: this->matchResp(evt); break;
	}
}
#pragma endregion

#pragma region private
void MatchClientHandler::matchResp(S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	match::Match_Resp respFromMatch = {};
	lobby::Match_Resp lobbyResp = {};

	respFromMatch.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen);
	lobbyResp.set_ip(respFromMatch.ip());
	lobbyResp.set_port(respFromMatch.port());

	auto serials = respFromMatch.serials();
	for (int i = 0; i < serials.size(); ++i)
	{
		Server::getInstance().send(E_EngineType::LOBBY_SERVER, serials[i], { (UINT16)lobby::E_PacketID::MATCH_RESP, 0 }, lobbyResp);
		UserManager::getInstance().getUser(serials[i]).state = E_UserState::INGAME;
	}
}
#pragma endregion