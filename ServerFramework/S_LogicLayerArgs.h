#pragma once
#include "Define.h"
#include "E_EngineEvent.h"

struct S_LogicLayerArgs
{
	int serial;
	E_EngineEvent type; // 어떤 이벤트인지 ( EVENT_NET_CONNECT ... )
	Size blockSize;
	char* data = nullptr; // 헤더 + body
};