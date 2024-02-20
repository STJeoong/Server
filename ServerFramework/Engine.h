#pragma once
#include "Define.h"
#include "NetEvent.h"
#include "S_EngineEvent.h"

class I_NetworkCore;
class NetworkLayer;
class EngineEventContainer;
class Decoder;
class Encoder;
class Serializer;
class Engine
{
public:
	Engine(I_NetworkCore* network, int maxClient);
	~Engine();
	void run();
	void send(int to, Size blockSize, char* data);
	void disconnect(int serial);
	S_EngineEvent getEvent() const;
private:

	NetworkLayer* _network = nullptr;
	EngineEventContainer* _evtContainer = nullptr;
	Decoder* _decoder = nullptr;
	Encoder* _encoder = nullptr;
};