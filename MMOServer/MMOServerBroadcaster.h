#pragma once
#include <I_Broadcaster.h>
#include <Delegate.h>
#include "MMO_protocol.pb.h"

class MMOServerBroadcaster : public I_Broadcaster
{
public:
	// events
	
	// connection
	static Delegate<const int> onConnect;
	static Delegate<const int> onDisconnect;
	// packets
	static Delegate<const int> onEnterGameReq;
	static Delegate<const int, const protocol::mmo::Move_Req&> onMoveReq;
	static Delegate<const int> onIdleReq;
	static Delegate<const int> onNormalAttackReq;
	static Delegate<const int, const protocol::mmo::UseItem_Req&> onUseItemReq;
	// timer
	static Delegate<> onUpdate;
	static Delegate<> onMonsterRespawn;

public:
	virtual void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastMessage(const S_EngineEvent& evt);
	void broadcastTimer(const S_EngineEvent& evt);
};