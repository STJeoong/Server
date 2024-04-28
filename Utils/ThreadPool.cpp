#include "pch.h"
#include "ThreadPool.h"

#pragma region public
void ThreadPool::enqueue(std::function<void()> func)
{
	if (s_stopThreads)
		return;
	if (s_workers.empty()) ThreadPool::makeWorkers();
	std::lock_guard<std::mutex> lock(s_mutex);
	s_queue.push(std::move(func));
	s_cv.notify_one();
}
void ThreadPool::terminate()
{
	s_stopThreads = true;
	s_cv.notify_all();
	for (int i = 0; i < ThreadPool::THREAD_COUNT; ++i)
		s_workers[i].join();
}
#pragma endregion

#pragma region private
void ThreadPool::makeWorkers()
{
	for (int i = 0; i < ThreadPool::THREAD_COUNT; ++i)
		s_workers.push_back(std::thread(&ThreadPool::threadMain));
}
void ThreadPool::threadMain()
{
	std::function<void()> func;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(s_mutex);
			s_cv.wait(lock, [&]() { return s_stopThreads || !s_queue.empty(); });
			if (s_stopThreads)
				return;
			func = std::move(s_queue.front());
			s_queue.pop();
		}
		func();
	}
}
#pragma endregion

bool ThreadPool::s_stopThreads;
std::vector<std::thread> ThreadPool::s_workers;
std::mutex ThreadPool::s_mutex;
std::queue<std::function<void()>> ThreadPool::s_queue;
std::condition_variable ThreadPool::s_cv;