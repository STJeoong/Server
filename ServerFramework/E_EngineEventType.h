#pragma once
#include <basetsd.h>
enum class E_EngineEventType : UINT16
{
	NONE = 0,
	EVENT_NET_CONNECT = 1,
	EVENT_NET_DISCONNECT = 2,
	EVENT_NET_RECV = 3,
	EVENT_TIMER = 4,
};