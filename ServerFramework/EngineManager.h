#pragma once
#include <vector>
#include <thread>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Define.h"
#include "S_ServerConfig.h"
#include "S_EngineEvent.h"

class Engine;
class EngineEventContainer;
class EngineManager
{
public:
	EngineManager(const char* argv0);
	~EngineManager();
	void addEngine(int engineID, const S_ServerConfig& config, EngineEventContainer* container);
	void run();
	void run(int engineID);
	void shutdown(int engineID);
	void send(int engineID, int serial, Size blockSize, char* data);
	void disconnect(int engineID, int serial);
private:
	void setLogFolder();
	Engine* parseConfig(int engineID, const S_ServerConfig& config, EngineEventContainer* container);

	std::unordered_map<int, Engine*> _mp = {};
};

