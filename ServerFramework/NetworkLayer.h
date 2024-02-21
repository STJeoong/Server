#pragma once

#include "Define.h"
class I_NetworkCore;
class Decoder;
class EngineEventContainer;
class NetworkLayer
{
public:
	NetworkLayer(I_NetworkCore* core, EngineEventContainer* evtContainer, Decoder* decoder);
	~NetworkLayer();
	void run();
	void send(int to, Size blockSize, int len, char* data);
	void disconnect(int serial);
private:
	void onConnect(int serial);
	void onDisconnect(int serial);
	void onRecv(int serial, int len, char* data);

	I_NetworkCore* _core = nullptr;
	EngineEventContainer* _evtContainer = nullptr;
	Decoder* _decoder = nullptr;
};

