#pragma once
#include <nlohmann/json.hpp>

struct S_Stats
{
	int maxHp;
	int maxMp;
	int hp;
	int mp;

	int defaultAttack;
	int defaultDefence;
	int defaultSpeed;
	int attack;
	int defence;
	int speed;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_Stats, maxHp, maxMp, defaultAttack, defaultDefence, defaultSpeed)