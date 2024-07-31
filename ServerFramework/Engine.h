#pragma once
#include "Define.h"
#include "NetEvent.h"
#include "S_EngineEvent.h"
#include "S_ServerConfig.h"
#include "E_ServerMode.h"
#include "E_EngineState.h"

class I_NetworkCore;
class NetworkLayer;
class I_EngineEventContainer;
class Decoder;
class Encoder;
class I_Broadcaster;
class ServerMode;
class Engine
{
public:
	static const int MAX_ENGINE = 100;

	// must be executed eariler than other functions
	static void setLogFolder(const char* argv0);
	static void setServerMode(E_ServerMode mode);
	//
	static bool addEngine(int engineID, const S_ServerConfig& config, I_Broadcaster* broadcaster);
	static const S_ServerConfig& getEngineConfig(int engineID);
	static void runEngine();
	static void runEngine(int engineID);
	static void runServer();
	static void shutdown(int engineID);
	static void send(int engineID, int serial, Size blockSize, char* data);
	static void disconnect(int engineID, int serial);
	static void setTimer(int engineID, int serial, int millisec, int evt);
	static void setRepetitiveTimer(int engineID, int serial, int millisec, int evt);
private:
	static Engine* parseConfig(int engineID, const S_ServerConfig& config);

	Engine(int id, I_NetworkCore* core, int maxClient, I_EngineEventContainer* container);
	~Engine();
	void run();
	void send(int to, Size blockSize, char* data);
	void disconnect(int serial);

	static Engine* s_engines[Engine::MAX_ENGINE];
	static ServerMode* s_serverMode;
	static I_EngineEventContainer* s_evtContainer;

	E_EngineState _state = E_EngineState::INITIAL;
	S_ServerConfig _config = {};
	NetworkLayer* _network = nullptr;
	Decoder* _decoder = nullptr;
	Encoder* _encoder = nullptr;
};