#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "S_Stats.h"
#include "MMO_protocol.pb.h"

struct S_EquipmentData
{
	std::string equipmentName;
	int templateID;
	protocol::mmo::E_Equipment type;
	S_Stats deltaStats;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_EquipmentData, equipmentName, templateID, type, deltaStats)