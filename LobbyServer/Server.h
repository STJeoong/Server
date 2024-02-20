#pragma once
#include "S_ServerConfig.h"
#include "E_EngineType.h"
#include <Singleton.h>
#include <S_PacketAttr.h>
#include <google/protobuf/message.h>

class EngineManager;
class PacketHandler;
class Serializer;
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
	void setLogFolder();
	void setConfig();
	void setEngine();

	EngineManager* _engineManager = nullptr;
	PacketHandler* _packetHandler = nullptr;
	Serializer* _serializer = nullptr;
	nlohmann::json _json;
	S_ServerConfig _config = {};
};