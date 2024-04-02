#pragma once
#include <functional>
#include <I_Broadcaster.h>
#include <Delegate.h>
#include "match_protocol.pb.h"

using namespace protocol::match;
class MatchServerBroadcaster : public I_Broadcaster
{
public:
	static void onConnect(bool add, std::function<void(const int)> func) { if (add) s_onConnect += func; else s_onConnect -= func; }
	static void onDisconnect(bool add, std::function<void(const int)> func) { if (add) s_onDisconnect += func; else s_onDisconnect -= func; }
	static void onMatchReq(bool add, std::function<void(const int, const Match_Req&)> func) { if (add) s_onMatchReq += func; else s_onMatchReq -= func; }
	static void onMatchCancleReq(bool add, std::function<void(const int, const MatchCancle_Req&)> func) { if (add) s_onMatchCancleReq += func; else s_onMatchCancleReq -= func; }

	virtual void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastTimer(const S_EngineEvent& evt);
	void broadcastMessage(const S_EngineEvent& evt);

	// events
	static Delegate<const int> s_onConnect;
	static Delegate<const int> s_onDisconnect;
	static Delegate<const int, const Match_Req&> s_onMatchReq;
	static Delegate<const int, const MatchCancle_Req&> s_onMatchCancleReq;
};

