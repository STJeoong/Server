#pragma once
#include "Define.h"
#include "E_EngineEventType.h"

struct S_EngineEvent
{
	int serial;
	E_EngineEventType type; // � �̺�Ʈ���� ( EVENT_NET_CONNECT ... )
	Size blockSize;
	char* data = nullptr; // ��� + body
};