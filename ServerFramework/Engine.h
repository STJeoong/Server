#pragma once
#include "Define.h"
#include "NetEvent.h"
#include "S_RawData.h"
#include "ServerConfig.h"
#include "Singleton.h"
#include "S_PacketAttr.h"
#include "google/protobuf/type.pb.h"

class I_Server;
class NetworkLayer;
class LogicLayer;
class Decoder;
class Encoder;
class I_DBService;
class Serializer;
class Engine : public Singleton<Engine>
{
	friend class Singleton;
public:
	void init(const char* argv0, I_Server* server);
	void shutdown();
	void start();
	void send(int to, S_PacketAttr attr, const google::protobuf::Message& message);
	int getMaxClient() const { return MAX_CLIENT; }
private:
	Engine() = default;
	~Engine() = default;
	void setLogFolder();
	void setConfig();


	ServerConfig _config;
	NetworkLayer* _network = nullptr;
	LogicLayer* _logic = nullptr;
	Decoder* _decoder = nullptr;
	Encoder* _encoder = nullptr;
	Serializer* _serializer = nullptr;
};