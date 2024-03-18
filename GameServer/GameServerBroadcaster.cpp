#include "GameServerBroadcaster.h"
#include "PlayerMoveUpdater.h"
#include "GameManager.h"
#include "E_TimerEvent.h"
#include <S_PacketHeader.h>

#pragma region public
void GameServerBroadcaster::init()
{
	_updater = new PlayerMoveUpdater();
	_gameManager = new GameManager();
}
void GameServerBroadcaster::broadcast(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: _onConnect(evt.serial); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: _onDisconnect(evt.serial); return;
	case E_EngineEventType::EVENT_TIMER: this->broadcastTimer(evt); return;
	case E_EngineEventType::EVENT_NET_RECV: this->broadcastMessage(evt); return;
	}
}
#pragma endregion

#pragma region private
GameServerBroadcaster::~GameServerBroadcaster()
{
	delete _updater;
	delete _gameManager;
}
void GameServerBroadcaster::broadcastTimer(S_EngineEvent& evt)
{
	switch ((E_TimerEvent)((size_t)evt.data))
	{
	case E_TimerEvent::UPDATE_WORLD: _onUpdateWorldTime(); break;
	case E_TimerEvent::GAME_START: _onGameStart(); break;
	}
}
void GameServerBroadcaster::broadcastMessage(S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::MOVE_REQ: Move_Req req = {}; req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen - sizeof(S_PacketHeader)); _onMoveReq(evt.serial, req); break;
	}
}
#pragma endregion