#pragma once
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "Define.h"

class I_EngineEventContainer;
class PacketParser;
class Decoder
{
public:
	Decoder(int engineID, I_EngineEventContainer* evtContainer, int maxClient);
	~Decoder();
	void enqueue(int serial, char* data, int len, Size blockSize);
	void reset(int serial);
private:
	void threadMain();

	int _engineID;
	I_EngineEventContainer* _evtContainer;
	PacketParser* _parser;

	bool _stopThread = false;
	std::thread _thread;
	std::queue<std::tuple<int, char*, int, Size>> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
};

