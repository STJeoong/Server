#pragma once
#include <I_Broadcaster.h>
#include <Delegate.h>
#include "match_protocol.pb.h"

using namespace protocol;
class MatchClientBroadcaster : public I_Broadcaster
{
public:
	// events
	static void onConnect(bool add, std::function<void(const int)> func) { if (add) s_onConnect += func; else s_onConnect -= func; }
	static void onDisconnect(bool add, std::function<void(const int)> func) { if (add) s_onDisconnect += func; else s_onDisconnect -= func; }
	static void onMatchResp(bool add, std::function<void(const match::Match_Resp&)> func) { if (add) s_onMatchResp += func; else s_onMatchResp -= func; }

	void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastTimer(const S_EngineEvent& evt);
	void broadcastMessage(const S_EngineEvent& evt);

	// events
	static Delegate<const int> s_onConnect;
	static Delegate<const int> s_onDisconnect;
	static Delegate<const match::Match_Resp&> s_onMatchResp;
};

