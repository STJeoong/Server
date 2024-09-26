#pragma once
#include <nlohmann/json.hpp>

struct S_Stats
{
	int maxHp;
	int maxMp;
	int defaultAtk;
	int defaultDef;
	int defaultSpeed;

	int hp;
	int mp;
	int atk;
	int def;
	int speed;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_Stats, maxHp, maxMp, defaultAtk, defaultDef, defaultSpeed)