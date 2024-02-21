#include "pch.h"
#include "IOCPClient.h"
#include "Receiver.h"
#include "Sender.h"
#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma region public
IOCPClient::IOCPClient(std::string ip, u_short port)
{
	_cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (_cp == INVALID_HANDLE_VALUE)
		puts("CreateIOCompletionPort error");

	_completionKey.id = 0;
	if ((_completionKey.sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		puts("WSASocket error");
		return;
	}
	if (CreateIoCompletionPort((HANDLE)_completionKey.sock, _cp, (ULONG_PTR)&_completionKey, 0) == NULL)
	{
		puts("bind cp error");
		return;
	}
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr);

	sockaddr_in myAddr = {};
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(0);
	myAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(_completionKey.sock, (sockaddr*)&myAddr, sizeof(sockaddr_in));

	memset(&_conn.o, 0, sizeof(OVERLAPPED));
	_conn.mode = IOMode::CONNECT;
	_conn.sessionIdx = 0;
	_conn.wsa.len = 0;
	_conn.wsa.buf = nullptr;

	GUID GuidConnectEx = WSAID_CONNECTEX;
	LPFN_CONNECTEX lpfnConnectEx = NULL;
	DWORD dwBytes;
	WSAIoctl(_completionKey.sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidConnectEx, sizeof(GuidConnectEx),
		&lpfnConnectEx, sizeof(lpfnConnectEx),
		&dwBytes, NULL, NULL);
	lpfnConnectEx(_completionKey.sock, (sockaddr*)&_addr, sizeof(sockaddr_in), NULL, 0, NULL, (LPOVERLAPPED)&_conn);
	_receiver = new Receiver(&_completionKey, 1);
	_sender = new Sender(&_completionKey, 1);
}
IOCPClient::~IOCPClient()
{
	delete _receiver;
	delete _sender;
	if (_cp != INVALID_HANDLE_VALUE)
	{
		closesocket(_completionKey.sock);
		CloseHandle(_cp);
	}
	for (int i = 0; i < _workers.size(); ++i)
		_workers[i].join();
}
void IOCPClient::run(int threadCount) { this->createWorkerThread(threadCount); }
void IOCPClient::send(int to, Size blockSize, int len, char* data) { _sender->send(0, blockSize, len, data); }
void IOCPClient::disconnect(int idx) { this->notifyDisconnection(idx); }
void IOCPClient::setOnConnect(std::function<void(int)> onConnect) { _onConnect = onConnect; }
void IOCPClient::setOnDisconnect(std::function<void(int)> onDisconnect) { _onDisconnect = onDisconnect; }
void IOCPClient::setOnRecv(std::function<void(int, int, char*)> onRecv) { _onRecv = onRecv; }
#pragma endregion

#pragma region private
void IOCPClient::createWorkerThread(int threadCount)
{
	for (int i = 0; i < threadCount; ++i)
		_workers.emplace_back(&IOCPClient::threadMain, this, _cp);
}
void IOCPClient::threadMain(HANDLE cp)
{
	DWORD bytes;
	CompletionKey* pCompletionKey;
	OverlappedEx* pOverlapped;
	BOOL result;

	while (true)
	{
		result = GetQueuedCompletionStatus(cp, &bytes, (PULONG_PTR)&pCompletionKey, (LPOVERLAPPED*)&pOverlapped, INFINITE);
		if (result == FALSE && pOverlapped == NULL && WSAGetLastError() == ERROR_ABANDONED_WAIT_0) // cp ÇÚµéÀÌ closeµÊ
			return;
		if (WSAGetLastError() == ERROR_CONNECTION_REFUSED)
		{
			puts("connection refused");
			return;
		}
		if (pOverlapped == NULL)
			continue;
		switch (pOverlapped->mode)
		{
		case IOMode::CONNECT: this->onConnect(pOverlapped->sessionIdx); break;
		case IOMode::RECV: this->onRecv(*pCompletionKey, bytes); break;
		case IOMode::SEND: this->onSend(*pCompletionKey, bytes); break;
		}
	}
}
void IOCPClient::onConnect(int idx)
{
	_receiver->onConnect(idx);
	_onConnect(idx);
}
void IOCPClient::onRecv(CompletionKey& ck, DWORD bytes)
{
	if (bytes == 0)
	{
		this->notifyDisconnection(ck.id);
		return;
	}

	_onRecv(ck.id, bytes, _receiver->getBuf(ck.id));
	_receiver->onRecv(ck.id);
}
void IOCPClient::onSend(CompletionKey& ck, DWORD bytes) { _sender->onSend(ck.id, bytes); }
void IOCPClient::notifyDisconnection(int idx)
{
	_sender->onDisconnection(idx);
	_onDisconnect(idx);
	closesocket(_completionKey.sock);
	CloseHandle(_cp);

	/*_completionKey.sock = INVALID_SOCKET;
	_cp = INVALID_HANDLE_VALUE;*/
}
#pragma endregion


