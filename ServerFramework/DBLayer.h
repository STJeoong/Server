#pragma once
#include <unordered_map>
#include <condition_variable>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
class I_DBService;
class DBLayer
{
public:
	bool addService(int serviceId, I_DBService* service); // �Ű������� �������� �ޱ�
	void start(int threadCount = 4);
	bool enqueue(); // �Ű������� request
	void shutdown();
private:
	void threadMain();

	std::unordered_map<int, I_DBService*> _services;

	bool _stopThreads = false;
	std::vector<std::thread> _threads;
	std::queue<int> _queue;
	std::condition_variable _cv;
	std::mutex _mutex;
};

