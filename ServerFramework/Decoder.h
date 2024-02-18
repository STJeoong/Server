#pragma once
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "Define.h"

class EngineEventContainer;
class PacketParser;
class Decoder
{
public:
	Decoder(EngineEventContainer* logic, int threadCount = 1);
	~Decoder();
	void enqueue(int serial, char* data, int len, Size blockSize);
	void reset(int serial);
private:
	void threadMain();

	EngineEventContainer* _evtContainer;
	PacketParser* _parser;

	bool _stopThreads = false;
	std::vector<std::thread> _threads;
	std::queue<std::tuple<int, char*, int, Size>> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
};

