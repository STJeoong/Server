#pragma once
#include <I_Broadcaster.h>
#include <functional>
#include <Delegate.h>
#include "db_protocol.pb.h"

using namespace protocol;
class DBClientBroadcaster : public I_Broadcaster
{
public:
	// events
	static void onConnect(bool add, std::function<void(const int)> func) { if (add) s_onConnect += func; else s_onConnect -= func; }
	static void onDisconnect(bool add, std::function<void(const int)> func) { if (add) s_onDisconnect += func; else s_onDisconnect -= func; }
	static void onLoginResp(bool add, std::function<void(const db::Login_Resp&)> func) { if (add) s_onLoginResp += func; else s_onLoginResp -= func; }

	void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastTimer(const S_EngineEvent& evt);
	void broadcastMessage(const S_EngineEvent& evt);

	// events
	static Delegate<const int> s_onConnect;
	static Delegate<const int> s_onDisconnect;
	static Delegate<const db::Login_Resp&> s_onLoginResp;
};

