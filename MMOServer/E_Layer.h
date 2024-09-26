#pragma once

enum class E_Layer
{
	DEFAULT,
	// AOI 영역
	AOI,

	// Object 식별 영역
	OBJ_MIN,
	PLAYER_OBJ,
	MONSTER_OBJ,
	SKILL_OBJ,
	ITEM_OBj,
	OBJ_MAX,

	// 선공격 영역
	MONSTER_AGGRESSIVE,

	// 스킬 영역
	SKILL,
};