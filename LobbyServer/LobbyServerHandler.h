#pragma once
#include <S_EngineEvent.h>
class LobbyServerHandler
{
public:
	void handle(S_EngineEvent& evt);
private:
	void login(int serial, char* data);
	void broadcastLobby(int serial, char* data);
	void matchReq(S_EngineEvent& evt);
	void matchCancle(S_EngineEvent& evt);
};

