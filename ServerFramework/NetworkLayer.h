#pragma once

#include "Define.h"
#include "ServerConfig.h"
class I_NetworkCore;
class Decoder;
class LogicLayer;
class NetworkLayer
{
public:
	NetworkLayer(ServerConfig config, LogicLayer* logic, Decoder* decoder);
	~NetworkLayer();
	void start();
	void send(int to, Size blockSize, int len, char* data);
private:
	void onConnect(int serial);
	void onDisconnect(int serial);
	void onRecv(int serial, int len, char* data);

	I_NetworkCore* _network = nullptr;
	LogicLayer* _logic = nullptr;
	Decoder* _decoder = nullptr;
};

