#pragma once
#include <functional>
class I_NetworkCore
{
public:
	virtual ~I_NetworkCore() {}
	virtual void start(int threadCount = 4) = 0;
	virtual void send(int to, Size blockSize, int len, char* data) = 0;
	virtual void disconnect(int idx) = 0;

	virtual void setOnConnect(std::function<void(int)> onConnect) = 0;
	virtual void setOnDisconnect(std::function<void(int)> onDisconnect) = 0;
	virtual void setOnRecv(std::function<void(int, int, char*)> onRecv) = 0;
};