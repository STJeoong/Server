#include "pch.h"
#include "DBLayer.h"
#include "I_DBService.h"

#pragma region public
bool DBLayer::addService(int serviceId, I_DBService* service)
{
	if (_services.find(serviceId) != _services.end())
		return false;
	_services[serviceId] = service;
	return true;
}
void DBLayer::start(int threadCount)
{
	if (_services.empty())
		return;
	for (int i = 0; i < threadCount; ++i)
		_threads.push_back(std::thread(&DBLayer::threadMain, this));
}
bool DBLayer::enqueue()
{
	if (_services.empty())
		return false;
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(1);
	_cv.notify_one();
	return true;
}
void DBLayer::shutdown()
{
	if (_threads.empty())
		return;
	_stopThreads = true;
	_cv.notify_all();
	for (int i = 0; i < _threads.size(); ++i)
		_threads[i].join();
}
// TODO : enqueue에 request 매개변수로 받기
#pragma endregion

#pragma region private
// TODO : request에서 protocolId 추출해서 처리
void DBLayer::threadMain()
{
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [&] { return !_queue.empty() || _stopThreads; });
			if (_stopThreads)
				return;
			_queue.pop();
		}
	}
}
#pragma endregion