#pragma once
class PacketHandler
{
public:
	void handle(int serial, char* data);
private:
	void login(int serial, char* data);
	void broadcastLobby(int serial, char* data);
};

