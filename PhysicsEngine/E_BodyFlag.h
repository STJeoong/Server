#pragma once
#include <basetsd.h>

enum class E_BodyFlag : UINT8
{
	AWAKE = 0x0001,
	FIXED_ROTATION = 0x0002,
	AUTO_SLEEP = 0x0004,
};