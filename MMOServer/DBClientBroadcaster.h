#pragma once
#include <I_Broadcaster.h>
#include <Delegate.h>
class DBClientBroadcaster : public I_Broadcaster
{
public:
	// connection
	static Delegate<const int> onConnect;
	static Delegate<const int> onDisconnect;

public:
	// I_Broadcaster을(를) 통해 상속됨
	virtual void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastMessage(const S_EngineEvent& evt);
	void broadcastTimer(const S_EngineEvent& evt);
};