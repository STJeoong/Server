#include "MatchServerBroadcaster.h"
#include <S_PacketHeader.h>

#pragma region public
void MatchServerBroadcaster::broadcast(const S_EngineEvent& evt)
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
void MatchServerBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
}
void MatchServerBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::MATCH_REQ: { Match_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onMatchReq(evt.serial, req); } break;
	case E_PacketID::MATCH_CANCLE_REQ: { MatchCancle_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onMatchCancleReq(evt.serial, req); }break;
	}
}
#pragma endregion

Delegate<const int> MatchServerBroadcaster::s_onConnect;
Delegate<const int> MatchServerBroadcaster::s_onDisconnect;
Delegate<const int, const Match_Req&> MatchServerBroadcaster::s_onMatchReq;
Delegate<const int, const MatchCancle_Req&> MatchServerBroadcaster::s_onMatchCancleReq;