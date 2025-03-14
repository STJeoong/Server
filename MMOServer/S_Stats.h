#pragma once
#include <nlohmann/json.hpp>

struct S_Stats
{
	int maxHp;
	int maxMp;
	int atk;
	int def;
	int speed;

	int hp;
	int mp;

	void operator+=(S_Stats delta)
	{
		maxHp = maxHp + delta.maxHp;
		maxMp = maxMp + delta.maxMp;
		atk = atk + delta.atk;
		def = def + delta.def;
		speed = speed + delta.speed;

		hp = hp + delta.hp;
		mp = mp + delta.mp;
	}
	S_Stats operator*(int value)
	{
		S_Stats ret = {};

		ret.maxHp = maxHp * value;
		ret.maxMp = maxMp * value;
		ret.atk = atk * value;
		ret.def = def * value;
		ret.speed = speed * value;
		ret.hp = hp * value;
		ret.mp = mp * value;

		return ret;
	}
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_Stats, maxHp, maxMp, atk, def, speed, hp, mp)