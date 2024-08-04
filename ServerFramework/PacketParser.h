#pragma once
#include <mutex>
#include "Define.h"
#include "NetEvent.h"
#include "S_EngineEvent.h"

class PacketParser
{
	static const UINT16 BUF_SIZE = 8192;
public:
	PacketParser(int maxClient);
	void pushData(int serial, char* data, int len);
	void collectData(int idx, S_EngineEvent& evt);
private:
	void resetBuf(int idx);

	UINT16* _readIdx = nullptr;
	UINT16* _writeIdx = nullptr;
	char** _buf = nullptr;
};


