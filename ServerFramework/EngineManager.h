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
	std::pair<int, S_EngineEvent> getEvent();
private:
	void threadMain(int engineID);

	bool _stopThreads = false;
	std::vector<std::thread> _threads = {};
	std::unordered_map<int, Engine*> _mp = {};
	std::queue<std::pair<int, S_EngineEvent>> _queue = {};
	std::mutex _mutex = {};
	std::condition_variable _cv = {};
};

