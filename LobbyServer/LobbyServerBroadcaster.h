#pragma once
#include <I_Broadcaster.h>
#include <Delegate.h>
#include <functional>
#include "lobby_protocol.pb.h"

using namespace protocol;
class LobbyServerBroadcaster : public I_Broadcaster
{
public:
	//events
	static void onConnect(bool add, std::function<void(const int)> func) { if (add) s_onConnect += func; else s_onConnect -= func; }
	static void onDisconnect(bool add, std::function<void(const int)> func) { if (add) s_onDisconnect += func; else s_onDisconnect -= func; }
	static void onLoginReq(bool add, std::function<void(const int, const lobby::Login_Req&)> func) { if (add) s_onLoginReq += func; else s_onLoginReq -= func; }
	static void onChatLobbyReq(bool add, std::function<void(const int, const lobby::ChatLobby_Req&)> func) { if (add) s_onChatLobbyReq += func; else s_onChatLobbyReq -= func; }
	static void onMatchReq(bool add, std::function<void(const int)> func) { if (add) s_onMatchReq += func; else s_onMatchReq -= func; }
	static void onMatchCancleReq(bool add, std::function<void(const int)> func) { if (add) s_onMatchCancleReq += func; else s_onMatchCancleReq -= func; }

	void broadcast(const S_EngineEvent& evt) override;
private:
	void broadcastTimer(const S_EngineEvent& evt);
	void broadcastMessage(const S_EngineEvent& evt);

	//events
	static Delegate<const int> s_onConnect;
	static Delegate<const int> s_onDisconnect;
	static Delegate<const int, const lobby::Login_Req&> s_onLoginReq;
	static Delegate<const int, const lobby::ChatLobby_Req&> s_onChatLobbyReq;
	static Delegate<const int> s_onMatchReq;
	static Delegate<const int> s_onMatchCancleReq;
};