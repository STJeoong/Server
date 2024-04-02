#pragma once
#include <S_EngineEvent.h>
#include <Delegate.h>
#include <functional>
#include <I_Broadcaster.h>
#include "game_protocol.pb.h"

using namespace protocol::game;
class GameServerBroadcaster : public I_Broadcaster
{
public:
	// events
	static void onConnect(bool add, std::function<void(int)> func) { if (add) s_onConnect += func; else s_onConnect -= func; }
	static void onDisconnect(bool add, std::function<void(int)> func) { if (add) s_onDisconnect += func; else s_onDisconnect -= func; }
	static void onGameStart(bool add, std::function<void()> func) { if (add) s_onGameStart += func; else s_onGameStart -= func; }
	static void onUpdateWorldTime(bool add, std::function<void()> func) { if (add) s_onUpdateWorldTime += func; else s_onUpdateWorldTime -= func; }
	static void onMoveReq(bool add, std::function<void(int, const Move_Req&)> func) { if (add) s_onMoveReq += func; else s_onMoveReq -= func; }
	static void onBombPlantReq(bool add, std::function<void(int)> func) { if (add) s_onBombPlantReq += func; else s_onBombPlantReq -= func; }

	void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastTimer(const S_EngineEvent& evt);
	void broadcastMessage(const S_EngineEvent& evt);

	// events
	static Delegate<const int> s_onConnect;
	static Delegate<const int> s_onDisconnect;
	static Delegate<> s_onGameStart;
	static Delegate<> s_onUpdateWorldTime;
	static Delegate<const int, const Move_Req&> s_onMoveReq;
	static Delegate<const int> s_onBombPlantReq;
};

