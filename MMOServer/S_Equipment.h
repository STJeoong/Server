#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "S_Stats.h"

struct S_Equipment
{
	std::string equipmentName;
	int templateID;
	S_Stats deltaStats;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_Equipment, equipmentName, templateID, deltaStats)