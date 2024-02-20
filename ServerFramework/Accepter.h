#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include "Define.h"
#include "OverlappedEx.h"

class Accepter
{
	static const UINT16 ACPT_BUF_SIZE = 64;
	static const UINT64 RE_USE_SESSION_WAIT_TIME_MILLI_SEC = 3000;
	static const UINT64 TOLERANT_TIME_MILLI_SEC = 50;
public:
	Accepter(SOCKET listenSock, HANDLE cp, CompletionKey* completionKeys, int maxClient, int threadAmount = 1, int waitTime = Accepter::RE_USE_SESSION_WAIT_TIME_MILLI_SEC);
	~Accepter();
	Accepter(const Accepter& obj) = delete;
	Accepter(Accepter&& obj) = delete;
	Accepter& operator=(const Accepter& obj) = delete;
	Accepter& operator=(Accepter&& obj) = delete;

	void onAccept(int idx);
	void onDisconnection(int idx);
private:
	void init(int maxClient);
	void threadMain();
	void pendingAccept(int idx);



	SOCKET _listenSock;
	HANDLE _cp;
	int _waitTime;

	CompletionKey* _completionKeys = nullptr;
	int _maxClient = 0;
	OverlappedEx* _acpts = nullptr;
	char** _acptBufs = nullptr;
	std::chrono::system_clock::time_point* _timePoints = nullptr;

	bool _stopThread = false;
	std::vector<std::thread> _workers;

	std::queue<int> _idxQueue; // 재등록이 필요한 소켓 번호들
	std::mutex _queueMutex;
	std::condition_variable _queueCv;
};

