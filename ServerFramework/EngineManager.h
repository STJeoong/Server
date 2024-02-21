#pragma once
#include <vector>
#include <thread>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Define.h"
#include "S_EngineEvent.h"

class Engine;
class EngineManager
{
public:
	EngineManager() = default;
	~EngineManager();
	void addEngine(int engineID, Engine* engine);
	void run();
	void run(int engineID);
	void shutdown(int engineID);
	void send(int engineID, int serial, Size blockSize, char* data);
	void disconnect(int engineID, int serial);
private:

	std::unordered_map<int, Engine*> _mp = {};
};

