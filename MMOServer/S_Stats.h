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
		maxHp = (maxHp + delta.maxHp) < 0 ? 0 : (maxHp + delta.maxHp);
		maxMp = (maxMp + delta.maxMp) < 0 ? 0 : (maxMp + delta.maxMp);
		atk = (atk + delta.atk) < 0 ? 0 : (atk + delta.atk);
		def = (def + delta.def) < 0 ? 0 : (def + delta.def);
		speed = (speed + delta.speed) < 0 ? 0 : (speed + delta.speed);

		hp = (hp + delta.hp) < 0 ? 0 : (hp + delta.hp);
		mp = (mp + delta.mp) < 0 ? 0 : (mp + delta.mp);
	}
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_Stats, maxHp, maxMp, atk, def, speed, hp, mp)