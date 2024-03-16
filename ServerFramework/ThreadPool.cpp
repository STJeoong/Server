#include "pch.h"
#include "ThreadPool.h"

#pragma region public
void ThreadPool::enqueue(std::function<void()> func)
{
	if (_stopThreads)
		return;
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(std::move(func));
	_cv.notify_one();
}
#pragma endregion

#pragma region private
ThreadPool::ThreadPool()
{
	for (int i = 0; i < ThreadPool::THREAD_COUNT; ++i)
		_workers.push_back(std::thread(&ThreadPool::threadMain, this));
}
ThreadPool::~ThreadPool()
{
	_stopThreads = true;
	_cv.notify_all();
	for (int i = 0; i < ThreadPool::THREAD_COUNT; ++i)
		_workers[i].join();
}
void ThreadPool::threadMain()
{
	std::function<void()> func;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [&]() { return _stopThreads || !_queue.empty(); });
			if (_stopThreads)
				return;
			func = std::move(_queue.front());
			_queue.pop();
		}
		func();
	}
}
#pragma endregion