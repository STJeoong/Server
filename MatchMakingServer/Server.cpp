#include "Server.h"
#include "MatchServerHandler.h"
#include "Serializer.h"
#include <EngineManager.h>
#include <fstream>
#include <MemoryBlockPool.h>
#include <EngineEventContainer.h>

#pragma region public
void Server::init(const char* argv0)
{
	_engineManager = new EngineManager(argv0);
	_evtContainer = new EngineEventContainer;
	_matchHandler = new MatchServerHandler;
	_serializer = new Serializer;

	this->setEngine();
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
		case E_EngineType::MATCH_SERVER: _matchHandler->handle(evt); break;
		}
		if (evt.data != nullptr)
			MemoryBlockPool::release(evt.blockSize, evt.data);
	}
}
void Server::send(E_EngineType type, int serial, S_PacketAttr attr, const google::protobuf::Message& messsage) const
{
	std::pair<Size, char*> val = _serializer->serialize(attr, messsage);
	_engineManager->send((int)type, serial, val.first, val.second);
}
#pragma endregion

#pragma region private
void Server::setEngine()
{
	std::ifstream f("match_config.json");
	json data = json::parse(f);

	data["server"].get_to(_config);

	_engineManager->addEngine((int)E_EngineType::MATCH_SERVER, _config, _evtContainer);
}
#pragma endregion