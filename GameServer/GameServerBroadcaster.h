#pragma once
#include <S_EngineEvent.h>
#include <Singleton.h>
#include <Delegate.h>
#include <functional>
#include "game_protocol.pb.h"

using namespace protocol::game;
class PlayerMoveUpdater;
class GameManager;
class GameServerBroadcaster : public Singleton<GameServerBroadcaster>
{
	friend class Singleton;
public:
	void onConnect(bool add, std::function<void(int)> func) { if (add) _onConnect += func; else _onConnect -= func; }
	void onDisconnect(bool add, std::function<void(int)> func) { if (add) _onDisconnect += func; else _onDisconnect -= func; }
	void onGameStart(bool add, std::function<void()> func) { if (add) _onGameStart += func; else _onGameStart -= func; }
	void onUpdateWorldTime(bool add, std::function<void()> func) { if (add) _onUpdateWorldTime += func; else _onUpdateWorldTime -= func; }
	void onMoveReq(bool add, std::function<void(int, Move_Req&)> func) { if (add) _onMoveReq += func; else _onMoveReq -= func; }

	void init();
	void broadcast(S_EngineEvent& evt);
private:
	GameServerBroadcaster() = default;
	~GameServerBroadcaster();
	void broadcastTimer(S_EngineEvent& evt);
	void broadcastMessage(S_EngineEvent& evt);

	PlayerMoveUpdater* _updater = nullptr;
	GameManager* _gameManager = nullptr;

	// events
	Delegate<int> _onConnect;
	Delegate<int> _onDisconnect;
	Delegate<> _onGameStart;
	Delegate<> _onUpdateWorldTime;
	Delegate<int, Move_Req&> _onMoveReq;
};

