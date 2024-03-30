#include "Server.h"
#include "LobbyServerHandler.h"
#include "DBClientHandler.h"
#include "MatchClientHandler.h"
#include "Serializer.h"
#include "E_EngineType.h"
#include <EngineManager.h>
#include <fstream>
#include <MemoryBlockPool.h>
#include <EngineEventContainer.h>

#pragma region public
void Server::init(const char* argv0)
{
	_engineManager = new EngineManager(argv0);
	_evtContainer = new EngineEventContainer;
	_lobbyHandler = new LobbyServerHandler;
	_dbHandler = new DBClientHandler;
	_matchHandler = new MatchClientHandler;
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
		case E_EngineType::LOBBY_SERVER: _lobbyHandler->handle(evt); break;
		case E_EngineType::DB_CLIENT: _dbHandler->handle(evt); break;
		case E_EngineType::MATCH_CLIENT: _matchHandler->handle(evt); break;
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
	std::ifstream f("config.json");
	json data = json::parse(f);
	S_ServerConfig dbConfig = {};
	S_ServerConfig matchConfig = {};

	data["server"].get_to(_config);
	data["db_client"].get_to(dbConfig);
	data["match_client"].get_to(matchConfig);

	_engineManager->addEngine((int)E_EngineType::LOBBY_SERVER, _config, _evtContainer);
	_engineManager->addEngine((int)E_EngineType::DB_CLIENT, dbConfig, _evtContainer);
	_engineManager->addEngine((int)E_EngineType::MATCH_CLIENT, matchConfig, _evtContainer);
}
#pragma endregion