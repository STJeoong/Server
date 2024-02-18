#pragma once
#include "Define.h"
#include "E_EngineEventType.h"

struct S_EngineEvent
{
	int serial;
	E_EngineEventType type; // 어떤 이벤트인지 ( EVENT_NET_CONNECT ... )
	Size blockSize;
	char* data = nullptr; // 헤더 + body
};