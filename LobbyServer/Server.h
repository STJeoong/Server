#pragma once
#include "S_ServerConfig.h"
#include "E_EngineType.h"
#include <Singleton.h>
#include <S_PacketAttr.h>
#include <google/protobuf/message.h>

class EngineManager;
class LobbyServerHandler;
class DBClientHandler;
class Serializer;
class EngineEventContainer;
class Server : public Singleton<Server>
{
	friend class Singleton;
public:
	void init(const char* argv0);
	void run();
	void send(E_EngineType type, int serial, S_PacketAttr attr, const google::protobuf::Message& message) const;
	int getMaxClient() const { return _config.maxClient; }
private:
	Server() = default;
	~Server() = default;
	void setEngine();

	EngineManager* _engineManager = nullptr;
	EngineEventContainer* _evtContainer = nullptr;
	LobbyServerHandler* _lobbyHandler = nullptr;
	DBClientHandler* _dbHandler = nullptr;
	Serializer* _serializer = nullptr;
	S_ServerConfig _config = {};
};