#include "GameServerBroadcaster.h"
#include "PlayerMoveUpdater.h"
#include "BombUpdater.h"
#include "GameManager.h"
#include "E_TimerEvent.h"
#include <S_PacketHeader.h>

#pragma region public
void GameServerBroadcaster::broadcast(const S_EngineEvent& evt)
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
void GameServerBroadcaster::broadcastTimer(const S_EngineEvent& evt)
{
	switch ((E_TimerEvent)((size_t)evt.data))
	{
	case E_TimerEvent::UPDATE_WORLD: s_onUpdateWorldTime(); break;
	case E_TimerEvent::GAME_START: s_onGameStart(); break;
	}
}
void GameServerBroadcaster::broadcastMessage(const S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::MOVE_REQ: { Move_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); s_onMoveReq(evt.serial, req); } break;
	case E_PacketID::PLANT_BOMB_REQ: { s_onBombPlantReq(evt.serial); } break;
	}
}
#pragma endregion

Delegate<const int> GameServerBroadcaster::s_onConnect;
Delegate<const int> GameServerBroadcaster::s_onDisconnect;
Delegate<> GameServerBroadcaster::s_onGameStart;
Delegate<> GameServerBroadcaster::s_onUpdateWorldTime;
Delegate<const int, const Move_Req&> GameServerBroadcaster::s_onMoveReq;
Delegate<const int> GameServerBroadcaster::s_onBombPlantReq;