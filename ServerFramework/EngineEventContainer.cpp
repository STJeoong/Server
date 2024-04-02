#include "pch.h"
#include "EngineEventContainer.h"

#pragma region public
EngineEventContainer::EngineEventContainer(E_EngineEventPopMode popMode)
{
	switch (popMode)
	{
	case E_EngineEventPopMode::WAIT_MODE: _popMode = new WaitMode(_queue, _mutex); break;
	case E_EngineEventPopMode::NO_WAIT_MODE: _popMode = new NoWaitMode(_queue, _mutex); break;
	}
}
#pragma endregion

#pragma region private
void EngineEventContainer::WaitMode::enqueue(int engineID, S_EngineEvent evt)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push({ engineID, evt });
	_cv.notify_one();
}
std::pair<int, S_EngineEvent> EngineEventContainer::WaitMode::pop()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_cv.wait(lock, [&] { return !_queue.empty(); });
	std::pair<int, S_EngineEvent> ret = _queue.front();
	_queue.pop();
	return ret;
}
void EngineEventContainer::NoWaitMode::enqueue(int engineID, S_EngineEvent evt)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push({ engineID, evt });
}
std::pair<int, S_EngineEvent> EngineEventContainer::NoWaitMode::pop()
{
	std::lock_guard<std::mutex> lock(_mutex);
	std::pair<int, S_EngineEvent> ret = {};
	if (_queue.empty())
		ret.second.type = E_EngineEventType::NONE;
	else
	{
		ret = _queue.front();
		_queue.pop();
	}
	return ret;
}
#pragma endregion