#include "pch.h"
#include "Accepter.h"
#include <Ws2tcpip.h>
#include <mswsock.h>
#pragma region public
Accepter::Accepter(SOCKET listenSock, HANDLE cp, CompletionKey* completionKeys, int maxClient, int threadAmount, int waitTime)
	: _listenSock(listenSock), _cp(cp), _completionKeys(completionKeys), _waitTime(waitTime)
{
	_acpts = new OverlappedEx[maxClient];
	_acptBufs = new char*[maxClient];
	for (int i = 0; i < maxClient; ++i)
		_acptBufs[i] = new char[Accepter::ACPT_BUF_SIZE]{};
	_timePoints = new std::chrono::system_clock::time_point[maxClient]{};

	this->init(maxClient);
	for (int i = 0; i < threadAmount; ++i)
		_workers.push_back(std::thread(&Accepter::threadMain, this));
}
Accepter::~Accepter()
{
	_stopThread = true;
	_queueCv.notify_all();
	for (int i = 0; i < _workers.size(); ++i)
		_workers[i].join();
}
void Accepter::onAccept(int idx)
{
	//printf("AcceptCompletion : SessionIndex(%d)\n", idx);
	CreateIoCompletionPort((HANDLE)_completionKeys[idx].sock, _cp, (ULONG_PTR)&_completionKeys[idx], 0);
}
void Accepter::onCloseConnection(int idx)
{
	closesocket(_completionKeys[idx].sock);
	_completionKeys[idx].sock = INVALID_SOCKET;
	_timePoints[idx] = std::chrono::system_clock::now();

	std::lock_guard<std::mutex> lock(_queueMutex);
	_idxQueue.push(idx);
	_queueCv.notify_one();
}
#pragma endregion

#pragma region private
void Accepter::threadMain()
{
	int idx;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_queueMutex);
			_queueCv.wait(lock, [&]() { return !_idxQueue.empty() || _stopThread; });
			if (_stopThread)
				return;
			idx = _idxQueue.front();
			_idxQueue.pop();
		}

		auto lastClosedTime = _timePoints[idx];
		auto currentTime = std::chrono::system_clock::now();
		auto timeInterval = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastClosedTime);
		if (timeInterval.count() + Accepter::TOLERANT_TIME_MILLI_SEC < Accepter::RE_USE_SESSION_WAIT_TIME_MILLI_SEC)
			std::this_thread::sleep_for(std::chrono::milliseconds(Accepter::RE_USE_SESSION_WAIT_TIME_MILLI_SEC - timeInterval.count()));
		this->pendingAccept(idx);
	}
}
void Accepter::pendingAccept(int idx)
{
	//printf("pendingAccept. client idx: %d\n", idx);
	_completionKeys[idx].sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_completionKeys[idx].sock == INVALID_SOCKET)
	{
		puts("WSASocket error");
		return;
	}
	DWORD bytes = 0;
	DWORD flags = 0;
	BOOL ret = 	AcceptEx(_listenSock, _completionKeys[idx].sock, _acptBufs[idx], 0, sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16, &bytes, &_acpts[idx].o);
	if (ret == FALSE && WSAGetLastError() != WSA_IO_PENDING)
		puts("AcceptEx error");
}
void Accepter::init(int maxClient)
{
	if (CreateIoCompletionPort((HANDLE)_listenSock, _cp, 0, 0) == nullptr)
	{
		puts("bind listenSock cp error");
		return;
	}

	for (int i = 0; i < maxClient; ++i)
	{
		_completionKeys[i].id = i;
		_completionKeys[i].sock = INVALID_SOCKET;
		
		memset(&_acpts[i].o, 0, sizeof(OVERLAPPED));
		_acpts[i].wsa.len = 0;
		_acpts[i].wsa.buf = nullptr;
		_acpts[i].mode = IOMode::ACCEPT;
		_acpts[i].sessionIdx = i;

		this->pendingAccept(i);
	}
}
#pragma endregion