#include "Server.h"
#include "User.h"
#include "PacketHandler.h"
#include "UserManager.h"

#pragma region public
bool Server::init()
{
	_packetHandler = new PacketHandler();
	return true;
}
#include "Engine.h"
void Server::process(int serial, E_EngineEvent type, char* data)
{
	switch ((E_EngineEvent)type)
	{
	case E_EngineEvent::EVENT_NET_CONNECT: printf("client connect : (%d)\n", serial); UserManager::getInstance().getUser(serial)->connect(); break;
	case E_EngineEvent::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", serial); UserManager::getInstance().getUser(serial)->disconnect(); break;
	case E_EngineEvent::EVENT_NET_RECV: _packetHandler->handle(serial, data); break;
	case E_EngineEvent::EVENT_DB_RESP: break;
	}
}
#pragma endregion

#pragma region private
#pragma endregion