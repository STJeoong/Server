#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct S_SkillData
{
	std::string name;
	int coolTime;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_SkillData, name, coolTime)