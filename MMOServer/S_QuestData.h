#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "MMO_enum.pb.h"

struct S_QuestTaskData
{
	protocol::mmo::E_QuestTaskType type;
	std::string what; // templateID
	int cnt;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_QuestTaskData, type, what, cnt)
struct S_QuestData
{
	std::string questName;
	int templateID;
	std::vector<S_QuestTaskData> tasks;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_QuestData, questName, templateID, tasks)