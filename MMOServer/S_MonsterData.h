#pragma once
#include <nlohmann/json.hpp>
#include <tuple>
#include <string>
#include <vector>
#include "S_Stats.h"
#include "S_RectDefine.h"
#include <basetsd.h>

struct S_SpawnInfo
{
	std::string mapName;
	int howMany;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_SpawnInfo, mapName, howMany)
struct S_MonsterData
{
	std::string monsterName;
	UINT8 templateID;
	int extraCellSize;
	S_Stats stats;

	std::vector<std::string> skills;
	std::vector<S_SpawnInfo> spawnInfo; // �����ʿ� �� ���� ������Ű����

	// ������
	bool isAggressive;
	S_RectDefine aggressiveArea;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MonsterData, monsterName, templateID, extraCellSize, stats, skills, spawnInfo, isAggressive, aggressiveArea)