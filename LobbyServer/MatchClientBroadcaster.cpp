#include "MatchClientBroadcaster.h"
#include <S_PacketHeader.h>

using namespace protocol::match;
#pragma region public
void MatchClientBroadcaster::broadcast(const S_EngineEvent& evt)
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
void MatchClientBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
}
void MatchClientBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::MATCH_RESP: { Match_Resp resp = {}; resp.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onMatchResp(resp); } break;
	}
}
#pragma endregion

Delegate<const int> MatchClientBroadcaster::s_onConnect;
Delegate<const int> MatchClientBroadcaster::s_onDisconnect;
Delegate<const match::Match_Resp&> MatchClientBroadcaster::s_onMatchResp;