#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "S_EngineEvent.h"
class I_Server;
class Engine;
class LogicLayer
{
public:
	LogicLayer(I_Server* server) : _server(server) {}
	~LogicLayer();
	void start() { _thread = std::thread(&LogicLayer::threadMain, this); }
	void enqueue(S_EngineEvent args);
private:
	void threadMain();


	I_Server* _server = nullptr;

	bool stopThread = false;
	std::thread _thread;
	std::mutex _mutex;
	std::queue<S_EngineEvent> _queue;
	std::condition_variable _cv;
};

