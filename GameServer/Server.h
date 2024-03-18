#pragma once
#include <Singleton.h>
#include <EngineManager.h>
#include <EngineEventContainer.h>
#include <S_PacketAttr.h>
#include "E_TimerEvent.h"
#include <google/protobuf/message.h>

class Serializer;
class Server : public Singleton<Server>
{
	friend class Singleton;
public:
	void init(char* argv[]);
	void run();
	void send(int engineID, int serial, S_PacketAttr attr, const google::protobuf::Message& message);
	void send(int engineID, int serial, S_PacketAttr attr);
	void setTimer(int engineID, int serial, int milliTime, E_TimerEvent evt);
	void setRepetitiveTimer(int engineID, int serial, int milliTime, E_TimerEvent evt);
private:
	Server() = default;
	~Server() = default;
	void setEngine(char* argv[]);

	EngineManager* _engineManager = nullptr;
	EngineEventContainer* _evtContainer = nullptr;
	Serializer* _serializer = nullptr;
};

