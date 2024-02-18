#include "Server.h"
#include "User.h"
#include "PacketHandler.h"
#include "UserManager.h"
#include "Serializer.h"
#include <MemoryBlockPoolManager.h>

#pragma region public
void Server::init(const char* argv0)
{
	_engine = new Engine(argv0);
	_packetHandler = new PacketHandler;
	_serializer = new Serializer;
}
void Server::run()
{
	_engine->start();
	S_EngineEvent evt;
	while (true)
	{
		evt = _engine->getEvent();
		switch (evt.type)
		{
		case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial)->connect(); break;
		case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial)->disconnect(); break;
		case E_EngineEventType::EVENT_NET_RECV: _packetHandler->handle(evt.serial, evt.data); break;
		}
		if (evt.data != nullptr)
			MemoryBlockPoolManager::getInstance().release(evt.blockSize, evt.data);
	}
}
void Server::send(int to, S_PacketAttr attr, const google::protobuf::Message& messsage) const
{
	std::pair<Size, char*> val = _serializer->serialize(attr, messsage);

	_engine->send(to, val.first, val.second);
}
#pragma endregion

#pragma region private
#pragma endregion
	/*switch ((E_EngineEventType)type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", serial); UserManager::getInstance().getUser(serial)->connect(); break;
	case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", serial); UserManager::getInstance().getUser(serial)->disconnect(); break;
	case E_EngineEventType::EVENT_NET_RECV: _packetHandler->handle(serial, data); break;
	case E_EngineEventType::EVENT_DB_RESP: break;
	}*/