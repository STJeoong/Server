#pragma once

#include "Define.h"
class I_NetworkCore;
class Decoder;
class I_EngineEventContainer;
class NetworkLayer
{
public:
	NetworkLayer(int engineID, I_NetworkCore* core, I_EngineEventContainer* evtContainer, Decoder* decoder);
	~NetworkLayer();
	void start();
	void send(int to, Size blockSize, int len, char* data);
	void disconnect(int serial);
private:
	void onConnect(int serial);
	void onDisconnect(int serial);
	void onRecv(int serial, int len, char* data);

	int _engineID;
	I_NetworkCore* _core = nullptr;
	I_EngineEventContainer* _evtContainer = nullptr;
	Decoder* _decoder = nullptr;
};

