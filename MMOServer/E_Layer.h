#pragma once

enum class E_Layer
{
	DEFAULT,
	// AOI ����
	AOI,

	// Object �ĺ� ����
	OBJ_MIN,
	PLAYER_OBJ,
	MONSTER_OBJ,
	PROJECTILE_OBJ,
	ITEM_OBj,
	OBJ_MAX,

	// ������ ����
	MONSTER_AGGRESSIVE,

	// ��ų ����
	PLAYER_SKILL,
	MONSTER_SKILL,
};