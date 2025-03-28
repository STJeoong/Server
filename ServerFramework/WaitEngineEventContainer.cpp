#include "pch.h"
#include "WaitEngineEventContainer.h"

#pragma region public
void WaitEngineEventContainer::enqueue(int engineID, S_EngineEvent evt)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push({ engineID, evt });
	_cv.notify_one();
}
std::pair<int, S_EngineEvent> WaitEngineEventContainer::pop()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_cv.wait(lock, [&] { return !_queue.empty(); });
	std::pair<int, S_EngineEvent> ret = _queue.front();
	_queue.pop();
	return ret;
}
#pragma endregion

#pragma region private
#pragma endregion