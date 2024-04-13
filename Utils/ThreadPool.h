#pragma once
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>
#include "Singleton.h"
class ThreadPool : public Singleton<ThreadPool>
{
	static const int THREAD_COUNT = 50;
	friend class Singleton;
public:
	void enqueue(std::function<void()> func);
private:
	ThreadPool();
	~ThreadPool();
	void threadMain();

	bool _stopThreads = false;
	std::vector<std::thread> _workers;
	std::mutex _mutex;
	std::queue<std::function<void()>> _queue;
	std::condition_variable _cv;
};

