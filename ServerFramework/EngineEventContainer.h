#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "S_EngineEvent.h"
#include "E_EngineEventPopMode.h"
class EngineEventContainer
{
public:
	EngineEventContainer(E_EngineEventPopMode popMode = E_EngineEventPopMode::WAIT_MODE);
	~EngineEventContainer() { delete _popMode; }
	void enqueue(int engineID, S_EngineEvent evt) { _popMode->enqueue(engineID, evt); }
	std::pair<int, S_EngineEvent> pop() { return _popMode->pop(); }
private:
	class I_PopMode
	{
	public:
		virtual ~I_PopMode() = default;
		virtual void enqueue(int engineID, S_EngineEvent evt) = 0;
		virtual std::pair<int, S_EngineEvent> pop() = 0;
	};
	class WaitMode : public I_PopMode
	{
	public:
		WaitMode(std::queue<std::pair<int, S_EngineEvent>>& queue, std::mutex& mutex) : _queue(queue), _mutex(mutex) {}
		void enqueue(int engineID, S_EngineEvent evt) override;
		std::pair<int, S_EngineEvent> pop() override;
	private:
		std::queue<std::pair<int, S_EngineEvent>>& _queue;
		std::mutex& _mutex;
		std::condition_variable _cv;
	};
	class NoWaitMode : public I_PopMode
	{
	public:
		NoWaitMode(std::queue<std::pair<int, S_EngineEvent>>& queue, std::mutex& mutex) : _queue(queue), _mutex(mutex) {}
		void enqueue(int engineID, S_EngineEvent evt) override;
		std::pair<int, S_EngineEvent> pop() override;
	private:
		std::queue<std::pair<int, S_EngineEvent>>& _queue;
		std::mutex& _mutex;
	};

	std::mutex _mutex;
	std::queue<std::pair<int, S_EngineEvent>>_queue;
	I_PopMode* _popMode = nullptr;
};

