#include "Server.h"
#include "E_EngineType.h"
#include "Serializer.h"
#include "GameServerBroadcaster.h"
#include "E_TimerEvent.h"
#include <MemoryBlockPoolManager.h>
#include <ThreadPool.h>

#pragma region public
void Server::init(char* argv[])
{
	_engineManager = new EngineManager(argv[0]);
	_evtContainer = new EngineEventContainer();
	_serializer = new Serializer();

	GameServerBroadcaster::getInstance().init();

	this->setEngine(argv);
}
void Server::run()
{
	_engineManager->run();
	int type;
	S_EngineEvent evt;

	while (true)
	{
		std::tie(type, evt) = _evtContainer->pop();

		switch ((E_EngineType)type)
		{
		case E_EngineType::GAME_SERVER: GameServerBroadcaster::getInstance().broadcast(evt); break;
		}
		if (evt.type == E_EngineEventType::EVENT_NET_RECV)
			MemoryBlockPoolManager::getInstance().release(evt.blockSize, evt.data);
	}
}
void Server::send(int engineID, int serial, S_PacketAttr attr, const google::protobuf::Message& message)
{
	std::pair<Size, char*> ret = _serializer->serialize(attr, message);
	_engineManager->send(engineID, serial, ret.first, ret.second);
}
void Server::send(int engineID, int serial, S_PacketAttr attr)
{
	std::pair<Size, char*> ret = _serializer->serialize(attr);
	_engineManager->send(engineID, serial, ret.first, ret.second);
}
void Server::setTimer(int engineID, int serial, int milliTime, E_TimerEvent evt)
{
	ThreadPool::getInstance().enqueue([engineID, serial, milliTime, evt, this]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliTime));
		S_EngineEvent engineEvt = {};
		engineEvt.serial = serial;
		engineEvt.type = E_EngineEventType::EVENT_TIMER;
		engineEvt.data = (char*)evt;
		_evtContainer->enqueue(engineID, engineEvt); });
}
void Server::setRepetitiveTimer(int engineID, int serial, int milliTime, E_TimerEvent evt)
{
	ThreadPool::getInstance().enqueue([engineID, serial, milliTime, evt, this]() {
		S_EngineEvent engineEvt = {};
		engineEvt.serial = serial;
		engineEvt.type = E_EngineEventType::EVENT_TIMER;
		engineEvt.data = (char*)evt;
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(milliTime));
			_evtContainer->enqueue(engineID, engineEvt);
		}
		});
}
#pragma endregion

#pragma region private
void Server::setEngine(char* argv[])
{
	S_ServerConfig config = {};
	config.network = "iocp_server";
	config.maxClient = 2;
	config.ip = argv[1];
	config.port = atoi(argv[2]);

	_engineManager->addEngine((int)E_EngineType::GAME_SERVER, config, _evtContainer);
}
#pragma endregion