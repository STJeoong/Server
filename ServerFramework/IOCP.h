#pragma once

#include <WinSock2.h>
#include <mutex>
#include <queue>
#include <memory>
#include <functional>
#include "Define.h"
#include "OverlappedEx.h"
#include "I_NetworkCore.h"
#include "ServerConfig.h"

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")
class Accepter;
class Sender;

class IOCP : public I_NetworkCore
{
	static const UINT16 RECV_BUF_SIZE = 1024;
public:
	IOCP(const ServerConfig& config);
	~IOCP();
	void start(int threadCount = 4) override;
	void send(int to, Size blockSize, int len, char* data) override;
	void setOnConnect(std::function<void(int)> onConnect) override;
	void setOnDisconnect(std::function<void(int)> onDisconnect) override;
	void setOnRecv(std::function<void(int, int, char*)> onRecv) override;
private:
	void createWorkerThread(int threadCount);
	void threadMain(HANDLE cp);
	void onAccept(int idx);
	void notifyCloseConnection(int idx);
	void onRecv(CompletionKey& ck, DWORD bytes);
	void onSend(CompletionKey& ck, DWORD bytes);
	void pendingRecv(int idx);


	WSADATA _wsa = {};
	SOCKET _sock; // 서버 소켓
	sockaddr_in _addr = {};
	HANDLE _cp;

	std::vector<std::thread> _workers;
	CompletionKey _completionKeys[MAX_CLIENT];
	OverlappedEx _recvs[MAX_CLIENT];
	char _recvBufs[MAX_CLIENT][IOCP::RECV_BUF_SIZE];
	Accepter* _accepter;
	Sender* _sender;

	//events
	std::function<void(int)> _onConnect;
	std::function<void(int)> _onDisconnect;
	std::function<void(int, int, char*)> _onRecv;
};

