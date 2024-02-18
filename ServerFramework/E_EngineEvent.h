#pragma once
#include <basetsd.h>
enum class E_EngineEvent : UINT16
{
	EVENT_NET_CONNECT = 0,
	EVENT_NET_DISCONNECT = 1,
	EVENT_NET_RECV = 2,
	EVENT_DB_RESP = 3
};