#pragma once

#include <WinSock2.h>
#include <mutex>
#include <queue>
#include <memory>
#include <functional>
#include "Define.h"
#include "OverlappedEx.h"
#include "I_NetworkCore.h"

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")
class Accepter;
class Receiver;
class Sender;

class IOCPServer : public I_NetworkCore
{
public:
	IOCPServer(std::string ip, u_short port, int maxClient);
	~IOCPServer();
	void run(int threadCount = 4) override;
	void send(int to, Size blockSize, int len, char* data) override;
	void disconnect(int idx) override;
	void setOnConnect(std::function<void(int)> onConnect) override;
	void setOnDisconnect(std::function<void(int)> onDisconnect) override;
	void setOnRecv(std::function<void(int, int, char*)> onRecv) override;
private:
	void createWorkerThread(int threadCount);
	void threadMain(HANDLE cp);
	void onAccept(int idx);
	void onRecv(CompletionKey& ck, DWORD bytes);
	void onSend(CompletionKey& ck, DWORD bytes);
	void notifyDisconnection(int idx);


	WSADATA _wsa = {};
	SOCKET _sock; // 서버 소켓
	sockaddr_in _addr = {};
	HANDLE _cp;

	std::vector<std::thread> _workers;
	CompletionKey* _completionKeys = nullptr;
	Receiver* _receiver = nullptr;
	Accepter* _accepter = nullptr;
	Sender* _sender = nullptr;

	//events
	std::function<void(int)> _onConnect;
	std::function<void(int)> _onDisconnect;
	std::function<void(int, int, char*)> _onRecv;
};

