#include "pch.h"
#include <thread>
#include <WS2tcpip.h>
#include "IOCP.h"
#include "Accepter.h"
#include "Sender.h"

#pragma region public
IOCP::IOCP(const ServerConfig& config)
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
	{
		puts("WSAStartup error");
		return;
	}
	if ((_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket error\n");
		return;
	}
	for (int i = 0; i < MAX_CLIENT; ++i)
	{
		memset(&_recvs[i].o, 0, sizeof(OVERLAPPED));
		// recvs->wsa.buf 라고 해서 안됐었음...
		_recvs[i].wsa.len = IOCP::RECV_BUF_SIZE;
		_recvs[i].wsa.buf = _recvBufs[i];
		_recvs[i].mode = IOMode::RECV;
		_recvs[i].sessionIdx = i;
	}
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(config.port);
	if (config.ip == "")
		_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, config.ip.c_str(), &_addr.sin_addr);
	if (bind(_sock, (sockaddr*)&_addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		puts("bind error");
		return;
	}
	if (listen(_sock, SOMAXCONN) == SOCKET_ERROR)
	{
		puts("listen error");
		return;
	}
	_cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (_cp == INVALID_HANDLE_VALUE)
		puts("CreateIOCompletionPort error");

	_accepter = new Accepter(_sock, _cp, _completionKeys);
	_sender = new Sender(_completionKeys);
}
IOCP::~IOCP()
{
	delete _accepter;
	delete _sender;
	closesocket(_sock);
	CloseHandle(_cp);
	WSACleanup();
	for (int i = 0; i < _workers.size(); ++i)
		_workers[i].join();
}
void IOCP::start(int threadCount) { this->createWorkerThread(threadCount); }
void IOCP::send(int to, Size blockSize, int len, char* data) { _sender->send(to, blockSize, len, data); }
void IOCP::setOnConnect(std::function<void(int)> onConnect) { _onConnect = onConnect; }
void IOCP::setOnDisconnect(std::function<void(int)> onDisconnect) { _onDisconnect = onDisconnect; }
void IOCP::setOnRecv(std::function<void(int, int, char*)> onRecv) { _onRecv = onRecv; }
#pragma endregion

#pragma region private
void IOCP::createWorkerThread(int threadCount)
{
	for (int i = 0; i < threadCount; ++i)
		_workers.emplace_back(&IOCP::threadMain, this, _cp);
}
void IOCP::threadMain(HANDLE cp)
{
	DWORD bytes;
	CompletionKey* pCompletionKey;
	OverlappedEx* pOverlapped;
	BOOL result;

	while (true)
	{
		result = GetQueuedCompletionStatus(cp, &bytes, (PULONG_PTR)&pCompletionKey, (LPOVERLAPPED*)&pOverlapped, INFINITE);
		if (result == FALSE && pOverlapped == NULL && WSAGetLastError() == ERROR_ABANDONED_WAIT_0) // cp 핸들이 close됨
			return;
		switch (pOverlapped->mode)
		{
		case IOMode::ACCEPT: this->onAccept(pOverlapped->sessionIdx); break;
		case IOMode::RECV: this->onRecv(*pCompletionKey, bytes); break;
		case IOMode::SEND: this->onSend(*pCompletionKey, bytes); break;
		}
	}
}
void IOCP::onAccept(int idx)
{
	_accepter->onAccept(idx);
	this->pendingRecv(idx);
	_onConnect(idx);
}
void IOCP::notifyCloseConnection(int idx)
{
	_accepter->onCloseConnection(idx);
	_sender->onCloseConnection(idx);
	_onDisconnect(idx);
}
void IOCP::onRecv(CompletionKey& ck, DWORD bytes)
{
	if (bytes == 0)
	{
		this->notifyCloseConnection(ck.id);
		return;
	}

	_onRecv(ck.id, bytes, _recvBufs[ck.id]);
	this->pendingRecv(ck.id);
}
void IOCP::onSend(CompletionKey& ck, DWORD bytes)
{
	_sender->onSend(ck.id, bytes);
}
void IOCP::pendingRecv(int idx)
{
	CompletionKey& ck = _completionKeys[idx];
	DWORD bytes, flag = 0;

	WSARecv(ck.sock, &_recvs[idx].wsa, 1, &bytes, &flag, &_recvs[idx].o, NULL);
}
#pragma endregion