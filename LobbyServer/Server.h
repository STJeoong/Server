#pragma once
#include "S_ServerConfig.h"
#include <Engine.h>
#include <Singleton.h>
#include <S_PacketAttr.h>
#include <google/protobuf/message.h>

class PacketHandler;
class Serializer;
class Server : public Singleton<Server>
{
	friend class Singleton;
public:
	void init(const char* argv0);
	void run();
	void send(int to, S_PacketAttr attr, const google::protobuf::Message& message) const;
	int getMaxClient() const { return _config.maxClient; }
private:
	Server() = default;
	~Server() = default;
	void setLogFolder();
	void setConfig();

	Engine* _engine = nullptr;
	PacketHandler* _packetHandler = nullptr;
	Serializer* _serializer = nullptr;
	S_ServerConfig _config = {};
};