#pragma once

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "S_RawData.h"
class NetworkLayer;
class Encoder
{
public:
	Encoder(NetworkLayer* network, int threadCount = 1);
	~Encoder();
	void enqueue(S_RawData data);
private:
	void threadMain();

	NetworkLayer* _network;

	bool _stopThreads = false;
	std::vector<std::thread> _threads;
	std::queue<S_RawData> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
};

