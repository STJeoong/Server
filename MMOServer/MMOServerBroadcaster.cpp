#include "MMOServerBroadcaster.h"
#include "E_TimerEvent.h"
#include "S_PacketHeader.h"
#include "MMO_protocol.pb.h"

using namespace protocol::mmo;

#pragma region public
void MMOServerBroadcaster::broadcast(const S_EngineEvent& evt)
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
void MMOServerBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);

	switch ((E_PacketID)header->id)
	{
	case E_PacketID::ENTER_GAME_REQ: MMOServerBroadcaster::onEnterGameReq(evt.serial); break;
	}
}
void MMOServerBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
	switch ((E_TimerEvent)((size_t)evt.data))
	{
	case E_TimerEvent::UPDATE: MMOServerBroadcaster::onUpdate(); break;
	}
}
#pragma endregion

Delegate<const int> MMOServerBroadcaster::onConnect;
Delegate<const int> MMOServerBroadcaster::onDisconnect;
Delegate<const int> MMOServerBroadcaster::onEnterGameReq;
Delegate<> MMOServerBroadcaster::onUpdate;