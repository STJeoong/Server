#pragma once
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>
class ThreadPool
{
	static const int THREAD_COUNT = 50;
public:
	static void enqueue(std::function<void()> func);
	static void terminate();
private:
	ThreadPool() = default;
	~ThreadPool() = default;
	static void makeWorkers();
	static void threadMain();

	static bool s_stopThreads;
	static std::vector<std::thread> s_workers;
	static std::mutex s_mutex;
	static std::queue<std::function<void()>> s_queue;
	static std::condition_variable s_cv;
};

