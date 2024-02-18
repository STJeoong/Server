#pragma once
#include <mutex>
#include "Define.h"
#include "NetEvent.h"
#include "S_EngineEvent.h"

class PacketParser
{
	static const UINT16 BUF_SIZE = 8192;
public:
	void pushData(int serial, char* data, int len, S_EngineEvent& args);
	void resetBuf(int idx);
private:
	void collectData(int idx, S_EngineEvent& args);


	std::mutex _readWriteMutex[MAX_CLIENT];
	UINT16 _readIdx[MAX_CLIENT] = {};
	UINT16 _writeIdx[MAX_CLIENT] = {};
	char _buf[MAX_CLIENT][BUF_SIZE] = {}; // 일단 스택 메모리 사용
};


