#pragma once
#include "Define.h"
#include "E_EngineEvent.h"

struct S_LogicLayerArgs
{
	int serial;
	E_EngineEvent type; // � �̺�Ʈ���� ( EVENT_NET_CONNECT ... )
	Size blockSize;
	char* data = nullptr; // ��� + body
};