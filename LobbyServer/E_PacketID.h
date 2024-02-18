#pragma once
#include <basetsd.h>
enum class E_PacketID : UINT16
{
	LOGIN_REQUEST = 10,
	LOGIN_RESPONSE = 11,

	CHAT_LOBBY_REQUEST = 100,
	CHAT_LOBBY_NOTIFY = 110,
};