#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "S_EngineEvent.h"
class EngineEventContainer
{
public:
	void enqueue(S_EngineEvent evt);
	S_EngineEvent pop();
private:

	std::mutex _mutex;
	std::queue<S_EngineEvent> _queue;
	std::condition_variable _cv;
};

