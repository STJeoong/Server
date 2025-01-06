#pragma once
#include <string>
#include <basetsd.h>
#include "S_Stats.h"
struct S_PlayerData
{
	std::string vocationName;
	UINT8 templateID;
	S_Stats stats;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_PlayerData, vocationName, templateID, stats)