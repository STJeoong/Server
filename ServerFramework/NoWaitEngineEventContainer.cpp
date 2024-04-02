#include "pch.h"
#include "NoWaitEngineEventContainer.h"

#pragma region public
void NoWaitEngineEventContainer::enqueue(int engineID, S_EngineEvent evt)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push({ engineID, evt });
}
std::pair<int, S_EngineEvent> NoWaitEngineEventContainer::pop()
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

#pragma region private
#pragma endregion