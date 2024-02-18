#pragma once
#include <mutex>
#include "Define.h"
#include "NetEvent.h"
#include "S_LogicLayerArgs.h"

class PacketParser
{
	static const UINT16 BUF_SIZE = 8192;
public:
	void pushData(int serial, char* data, int len, S_LogicLayerArgs& args);
	void resetBuf(int idx);
private:
	void collectData(int idx, S_LogicLayerArgs& args);


	std::mutex _readWriteMutex[MAX_CLIENT];
	UINT16 _readIdx[MAX_CLIENT] = {};
	UINT16 _writeIdx[MAX_CLIENT] = {};
	char _buf[MAX_CLIENT][BUF_SIZE] = {}; // �ϴ� ���� �޸� ���
};


