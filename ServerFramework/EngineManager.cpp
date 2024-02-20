#include "pch.h"
#include "EngineManager.h"
#include "Engine.h"

#pragma region public
EngineManager::~EngineManager()
{
	for (auto val : _mp)
		delete val.second;
	_stopThreads = true;
	for (int i = 0; i < _threads.size(); ++i)
		_threads[i].join();
}
void EngineManager::addEngine(int engineID, Engine* engine)
{
	_mp[engineID] = engine;
	_threads.push_back(std::thread(&EngineManager::threadMain, this, engineID));
}
void EngineManager::run() { for (auto val : _mp) val.second->run(); }
void EngineManager::send(int engineID, int serial, Size blockSize, char* data) { _mp[engineID]->send(serial, blockSize, data); }
void EngineManager::disconnect(int engineID, int serial) { _mp[engineID]->disconnect(serial); }
std::pair<int, S_EngineEvent> EngineManager::getEvent()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_cv.wait(lock, [&] { return !_queue.empty(); });
	auto ret = _queue.front();
	_queue.pop();
	return ret;
}
#pragma endregion

#pragma region private
void EngineManager::threadMain(int engineID)
{
	S_EngineEvent evt;
	while (!_stopThreads)
	{
		evt = _mp[engineID]->getEvent();
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push({ engineID, evt });
		_cv.notify_one();
	}
}
#pragma endregion

