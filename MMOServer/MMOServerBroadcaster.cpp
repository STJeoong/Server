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
	case E_PacketID::MOVE_REQ: { Move_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); MMOServerBroadcaster::onMoveReq(evt.serial, req); break; }
	case E_PacketID::IDLE_REQ: MMOServerBroadcaster::onIdleReq(evt.serial); break;
	case E_PacketID::NORMAL_ATTACK_REQ: MMOServerBroadcaster::onNormalAttackReq(evt.serial); break;
	case E_PacketID::USE_ITEM_REQ: { UseItem_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); MMOServerBroadcaster::onUseItemReq(evt.serial, req); break; }
	}
}
void MMOServerBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
	switch ((E_TimerEvent)((size_t)evt.data))
	{
	case E_TimerEvent::UPDATE: MMOServerBroadcaster::onUpdate(); break;
	case E_TimerEvent::MONSTER_RESPAWN: MMOServerBroadcaster::onMonsterRespawn(); break;
	}
}
#pragma endregion

Delegate<const int> MMOServerBroadcaster::onConnect;
Delegate<const int> MMOServerBroadcaster::onDisconnect;

Delegate<const int> MMOServerBroadcaster::onEnterGameReq;
Delegate<const int, const Move_Req&> MMOServerBroadcaster::onMoveReq;
Delegate<const int> MMOServerBroadcaster::onIdleReq;
Delegate<const int> MMOServerBroadcaster::onNormalAttackReq;
Delegate<const int, const UseItem_Req&> MMOServerBroadcaster::onUseItemReq;

Delegate<> MMOServerBroadcaster::onUpdate;
Delegate<> MMOServerBroadcaster::onMonsterRespawn;