#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "I_EngineEventContainer.h"

class WaitEngineEventContainer : public I_EngineEventContainer
{
public:
	virtual void enqueue(int engineID, S_EngineEvent evt) override;
	virtual std::pair<int, S_EngineEvent> pop() override;
private:

	std::queue<std::pair<int, S_EngineEvent>> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
};

