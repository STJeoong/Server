#pragma once
#include <WinSock2.h>
#include "Define.h"
#include "I_NetworkCore.h"
class IOCPClient : public I_NetworkCore
{
public:
	IOCPClient(std::string ip, u_short port);
	~IOCPClient();
	virtual void run(int threadCount = 4) override;
	virtual void send(int to, Size blockSize, int len, char* data) override;
	virtual void setOnConnect(std::function<void(int)> onConnect) override;
	virtual void setOnDisconnect(std::function<void(int)> onDisconnect) override;
	virtual void setOnRecv(std::function<void(int, int, char*)> onRecv) override;
private:
};

