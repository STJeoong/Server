#include "DBClientBroadcaster.h"
#include <S_PacketHeader.h>

using namespace protocol::db;
#pragma region public
void DBClientBroadcaster::broadcast(const S_EngineEvent& evt)
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
void DBClientBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
}
void DBClientBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::LOGIN_RESPONSE: { Login_Resp resp = {}; resp.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onLoginResp(resp); } break;
	}
}
#pragma endregion

Delegate<const int> DBClientBroadcaster::s_onConnect;
Delegate<const int> DBClientBroadcaster::s_onDisconnect;
Delegate<const db::Login_Resp&> DBClientBroadcaster::s_onLoginResp;