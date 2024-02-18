#pragma once
#include <Engine.h>
#include <Singleton.h>
class PacketHandler;
class Server : public Singleton<Server>
{
	friend class Singleton;
public:
	void init(const char* argv0);
	void run();
	void send(int to, S_PacketAttr attr, const google::protobuf::Message& message) const;
	int getMaxClient() const { return _engine->getMaxClient(); }
private:
	Server() = default;
	~Server() = default;

	Engine* _engine = nullptr;
	PacketHandler* _packetHandler = nullptr;
};