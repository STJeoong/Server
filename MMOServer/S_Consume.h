#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "S_SkillData.h"

struct S_Consume
{
	std::string consumeName;
	int templateID;
	std::vector<S_TargetBasedAction> targetBasedActions;
	int maxCnt;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_Consume, consumeName, templateID, targetBasedActions, maxCnt)