#pragma once
#include <basetsd.h>
enum class E_EngineEventType : UINT16
{
	NONE,
	EVENT_NET_CONNECT,
	EVENT_NET_CONNECT_FAIL,
	EVENT_NET_DISCONNECT,
	EVENT_NET_RECV,
	EVENT_TIMER,
};