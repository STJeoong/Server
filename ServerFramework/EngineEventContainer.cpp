#include "pch.h"
#include "EngineEventContainer.h"

#pragma region public
void EngineEventContainer::enqueue(S_EngineEvent evt)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(evt);
	_cv.notify_one();
}
S_EngineEvent EngineEventContainer::pop()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_cv.wait(lock, [&] { return !_queue.empty(); });
	S_EngineEvent ret = _queue.front();
	_queue.pop();
	return ret;
}
#pragma endregion

#pragma region private
#pragma endregion