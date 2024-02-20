#include "pch.h"
#include "IOCPClient.h"
#include <MSWSock.h>

#pragma region public
IOCPClient::IOCPClient(std::string ip, u_short port)
{
}

IOCPClient::~IOCPClient()
{
}

void IOCPClient::run(int threadCount)
{
}

void IOCPClient::send(int to, Size blockSize, int len, char* data)
{
}

void IOCPClient::setOnConnect(std::function<void(int)> onConnect)
{
}

void IOCPClient::setOnDisconnect(std::function<void(int)> onDisconnect)
{
}

void IOCPClient::setOnRecv(std::function<void(int, int, char*)> onRecv)
{
}
#pragma endregion

#pragma region private
#pragma endregion


