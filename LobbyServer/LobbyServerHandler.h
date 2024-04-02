#pragma once
#include <S_EngineEvent.h>
#include "lobby_protocol.pb.h"
class LobbyServerHandler
{
public:
	LobbyServerHandler();
	~LobbyServerHandler();
private:
	void onLoginReq(int serial, const protocol::lobby::Login_Req& req);
	void broadcastLobby(int serial, const protocol::lobby::ChatLobby_Req& req);
	void onMatchReq(int serial);
	void onMatchCancle(int serial);
};