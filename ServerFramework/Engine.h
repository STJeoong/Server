#pragma once
#include "Define.h"
#include "NetEvent.h"
#include "S_EngineEvent.h"

class I_NetworkCore;
class NetworkLayer;
class EngineEventContainer;
class Decoder;
class Encoder;
class Engine
{
public:
	Engine(int id, I_NetworkCore* core, int maxClient, EngineEventContainer* container);
	~Engine();
	void run();
	void send(int to, Size blockSize, char* data);
	void disconnect(int serial);
private:

	NetworkLayer* _network = nullptr;
	Decoder* _decoder = nullptr;
	Encoder* _encoder = nullptr;
};