#pragma once

enum class E_Layer
{
	DEFAULT,
	// AOI 영역
	AOI,

	// Object 식별 영역
	PLAYER_OBJ,
	MONSTER_OBJ,
	PROJECTILE_OBJ,
	ITEM_OBj,

	// 선공격 영역
	MONSTER_AGGRESSIVE,

	// 스킬 영역
	PLAYER_SKILL,
	MONSTER_SKILL,
};