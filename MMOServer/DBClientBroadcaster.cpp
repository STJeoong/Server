#include "DBClientBroadcaster.h"
#include "S_PacketHeader.h"
#include "DB_enum.pb.h"

using namespace protocol::db;
#pragma region public
void DBClientBroadcaster::broadcast(const S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: onConnect(evt.serial); break;
	case E_EngineEventType::EVENT_NET_DISCONNECT: onDisconnect(evt.serial); break;
	case E_EngineEventType::EVENT_NET_RECV: this->broadcastMessage(evt); break;
	case E_EngineEventType::EVENT_TIMER: this->broadcastTimer(evt); break;
	}
}
#pragma endregion

#pragma region private
void DBClientBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);

	switch ((E_PacketID)header->id)
	{
	}
}
void DBClientBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
}
#pragma endregion

Delegate<const int> DBClientBroadcaster::onConnect;
Delegate<const int> DBClientBroadcaster::onDisconnect;