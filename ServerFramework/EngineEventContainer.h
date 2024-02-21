#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "S_EngineEvent.h"
class EngineEventContainer
{
public:
	void enqueue(int engineID, S_EngineEvent evt);
	std::pair<int, S_EngineEvent> pop();
private:

	std::mutex _mutex;
	std::queue<std::pair<int, S_EngineEvent>>_queue;
	std::condition_variable _cv;
};

