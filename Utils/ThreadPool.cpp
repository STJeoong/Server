#include "pch.h"
#include "ThreadPool.h"

#pragma region public
int ThreadPool::enqueue(std::function<void()> func)
{
	if (s_stopThreads)
		return;
	if (s_workers.empty()) ThreadPool::makeWorkers();
	std::lock_guard<std::mutex> lock(s_mutex);
	s_queue.push({ s_allocateIdx, std::move(func) });
	s_cv.notify_one();
	s_finished[s_allocateIdx] = false;
	int ret = s_allocateIdx;
	s_allocateIdx = (s_allocateIdx + 1) % ThreadPool::MAX_KEY;
	return ret;
}
void ThreadPool::wait(int key)
{
	while (!s_finished[key]);
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
	int key;
	std::function<void()> func;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(s_mutex);
			s_cv.wait(lock, [&]() { return s_stopThreads || !s_queue.empty(); });
			if (s_stopThreads)
				return;
			std::tie(key, func) = std::move(s_queue.front());
			s_queue.pop();
		}
		func();
		s_finished[key] = true;
	}
}
#pragma endregion

bool ThreadPool::s_stopThreads;
std::vector<std::thread> ThreadPool::s_workers;
std::mutex ThreadPool::s_mutex;
std::queue<std::pair<int, std::function<void()>>> ThreadPool::s_queue;
std::condition_variable ThreadPool::s_cv;
bool ThreadPool::s_finished[ThreadPool::MAX_KEY];
int ThreadPool::s_allocateIdx = 0;