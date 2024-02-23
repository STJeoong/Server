#pragma once
#include "S_ServerConfig.h"
#include "E_EngineType.h"
#include <Singleton.h>
#include <S_PacketAttr.h>
#include <google/protobuf/message.h>

class EngineManager;
class Serializer;
class EngineEventContainer;
class MatchServerHandler;
class Server : public Singleton<Server>
{
	friend class Singleton;
public:
	void init(const char* argv0);
	void run();
	void send(E_EngineType type, int serial, S_PacketAttr attr, const google::protobuf::Message& message) const;
	S_ServerConfig& getServerConfig() { return _config; }
private:
	Server() = default;
	~Server() = default;
	void setEngine();

	EngineManager* _engineManager = nullptr;
	EngineEventContainer* _evtContainer = nullptr;
	MatchServerHandler* _matchHandler = nullptr;
	Serializer* _serializer = nullptr;
	S_ServerConfig _config = {};
};