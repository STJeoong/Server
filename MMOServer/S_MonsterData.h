#pragma once
#include <nlohmann/json.hpp>

struct S_MonsterData
{
	int a;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MonsterData, a)