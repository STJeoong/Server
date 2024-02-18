#pragma once

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "Define.h"
class NetworkLayer;
class Encoder
{
public:
	Encoder(NetworkLayer* network, int threadCount = 1);
	~Encoder();
	void enqueue(int to, Size blockSize, char* data);
private:
	void threadMain();

	NetworkLayer* _network;

	bool _stopThreads = false;
	std::vector<std::thread> _threads;
	std::queue<std::tuple<int, Size, char*>> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
};

