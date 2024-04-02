#include "LobbyServerBroadcaster.h"
#include <S_PacketHeader.h>

using namespace protocol::lobby;
#pragma region public
void LobbyServerBroadcaster::broadcast(const S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: s_onConnect(evt.serial); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: s_onDisconnect(evt.serial); return;
	case E_EngineEventType::EVENT_TIMER: this->broadcastTimer(evt); return;
	case E_EngineEventType::EVENT_NET_RECV: this->broadcastMessage(evt); return;
	}
}
#pragma endregion

#pragma region private
void LobbyServerBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
}
void LobbyServerBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::LOGIN_REQUEST: { Login_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onLoginReq(evt.serial, req); } break;
	case E_PacketID::CHAT_LOBBY_REQUEST: { ChatLobby_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onChatLobbyReq(evt.serial, req); } break;
	case E_PacketID::MATCH_REQ: s_onMatchReq(evt.serial); break;
	case E_PacketID::MATCH_CANCLE_REQ: s_onMatchCancleReq(evt.serial); break;
	default: LOG(ERROR) << "Undefined PacketID : " << header->id; break;
	}
}
#pragma endregion

Delegate<const int> LobbyServerBroadcaster::s_onConnect;
Delegate<const int> LobbyServerBroadcaster::s_onDisconnect;
Delegate<const int, const Login_Req&> LobbyServerBroadcaster::s_onLoginReq;
Delegate<const int, const ChatLobby_Req&> LobbyServerBroadcaster::s_onChatLobbyReq;
Delegate<const int> LobbyServerBroadcaster::s_onMatchReq;
Delegate<const int> LobbyServerBroadcaster::s_onMatchCancleReq;