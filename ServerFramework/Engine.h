#pragma once
#include "Define.h"
#include "NetEvent.h"
#include "S_RawData.h"
#include "ServerConfig.h"
#include "S_PacketAttr.h"
#include "S_EngineEvent.h"
#include "google/protobuf/type.pb.h"

class NetworkLayer;
class EngineEventContainer;
class Decoder;
class Encoder;
class Serializer;
class Engine
{
public:
	Engine(const char* argv0);
	~Engine();
	void start();
	void send(int to, S_PacketAttr attr, const google::protobuf::Message& message);
	S_EngineEvent getEvent() const;
	int getMaxClient() const { return MAX_CLIENT; }
private:
	void setLogFolder();
	void setConfig();


	ServerConfig _config;
	NetworkLayer* _network = nullptr;
	EngineEventContainer* _evtContainer = nullptr;
	Decoder* _decoder = nullptr;
	Encoder* _encoder = nullptr;
	Serializer* _serializer = nullptr;
};