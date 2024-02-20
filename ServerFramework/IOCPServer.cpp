#include "pch.h"
#include <thread>
#include <WS2tcpip.h>
#include "IOCPServer.h"
#include "Accepter.h"
#include "Receiver.h"
#include "Sender.h"

#pragma region public
IOCPServer::IOCPServer(std::string ip, u_short port, int maxClient)
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
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	if (ip == "any")
		_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr);
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
	
	_completionKeys = new CompletionKey[maxClient];
	_accepter = new Accepter(_sock, _cp, _completionKeys, maxClient);
	_receiver = new Receiver(_completionKeys, maxClient);
	_sender = new Sender(_completionKeys, maxClient);
}
IOCPServer::~IOCPServer()
{
	delete _accepter;
	delete _receiver;
	delete _sender;
	closesocket(_sock);
	CloseHandle(_cp);
	WSACleanup();
	for (int i = 0; i < _workers.size(); ++i)
		_workers[i].join();
}
void IOCPServer::run(int threadCount) { this->createWorkerThread(threadCount); }
void IOCPServer::send(int to, Size blockSize, int len, char* data) { _sender->send(to, blockSize, len, data); }
void IOCPServer::disconnect(int idx) { this->notifyDisconnection(idx); }
void IOCPServer::setOnConnect(std::function<void(int)> onConnect) { _onConnect = onConnect; }
void IOCPServer::setOnDisconnect(std::function<void(int)> onDisconnect) { _onDisconnect = onDisconnect; }
void IOCPServer::setOnRecv(std::function<void(int, int, char*)> onRecv) { _onRecv = onRecv; }
#pragma endregion

#pragma region private
void IOCPServer::createWorkerThread(int threadCount)
{
	for (int i = 0; i < threadCount; ++i)
		_workers.emplace_back(&IOCPServer::threadMain, this, _cp);
}
void IOCPServer::threadMain(HANDLE cp)
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
		switch (pOverlapped->mode)
		{
		case IOMode::ACCEPT: this->onAccept(pOverlapped->sessionIdx); break;
		case IOMode::RECV: this->onRecv(*pCompletionKey, bytes); break;
		case IOMode::SEND: this->onSend(*pCompletionKey, bytes); break;
		}
	}
}
void IOCPServer::notifyDisconnection(int idx)
{
	_accepter->onDisconnection(idx);
	_sender->onDisconnection(idx);
	_onDisconnect(idx);
}
void IOCPServer::onAccept(int idx)
{
	_accepter->onAccept(idx);
	_receiver->onAccept(idx);
	_onConnect(idx);
}
void IOCPServer::onRecv(CompletionKey& ck, DWORD bytes)
{
	if (bytes == 0)
	{
		this->notifyDisconnection(ck.id);
		return;
	}

	_onRecv(ck.id, bytes, _receiver->getBuf(ck.id));
	_receiver->onRecv(ck.id);
}
void IOCPServer::onSend(CompletionKey& ck, DWORD bytes) { _sender->onSend(ck.id, bytes); }
#pragma endregion