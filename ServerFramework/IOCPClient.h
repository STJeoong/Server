#pragma once
#include <WinSock2.h>
#include <vector>
#include <thread>
#include "Define.h"
#include "I_NetworkCore.h"
#include "OverlappedEx.h"

class Receiver;
class Sender;
class IOCPClient : public I_NetworkCore
{
public:
	IOCPClient(std::string ip, u_short port);
	~IOCPClient();
	virtual void start(int threadCount = 4) override;
	virtual void send(int to, Size blockSize, int len, char* data) override;
	void disconnect(int idx) override;
	virtual void setOnConnect(std::function<void(int)> onConnect) override;
	virtual void setOnDisconnect(std::function<void(int)> onDisconnect) override;
	virtual void setOnRecv(std::function<void(int, int, char*)> onRecv) override;
private:
	void createWorkerThread(int threadCount);
	void threadMain(HANDLE cp);
	void onConnect(int idx);
	void onRecv(CompletionKey& ck, DWORD bytes);
	void onSend(CompletionKey& ck, DWORD bytes);
	void notifyDisconnection(int idx);

	sockaddr_in _addr = {};
	HANDLE _cp = INVALID_HANDLE_VALUE;

	std::vector<std::thread> _workers;
	CompletionKey _completionKey = {};
	OverlappedEx _conn = {};
	Receiver* _receiver = nullptr;
	Sender* _sender = nullptr;

	//events
	std::function<void(int)> _onConnect;
	std::function<void(int)> _onDisconnect;
	std::function<void(int, int, char*)> _onRecv;
};

