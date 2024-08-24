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
	// timer
	static Delegate<> onUpdate;


	virtual void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastMessage(const S_EngineEvent& evt);
	void broadcastTimer(const S_EngineEvent& evt);
};