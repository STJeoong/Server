#pragma once
#include <nlohmann/json.hpp>
#include <tuple>
#include <string>
#include <vector>
#include "S_Stats.h"
#include <basetsd.h>

struct S_MonsterData
{
	S_Stats stats;
	UINT8 templateID;
	int extraCellSize;

	// 선공몹
	bool isAggressive;
	int offsetY;
	int offsetX;
	int yExtension;
	int xExtension;

	std::vector<std::pair<std::string, int>> spawnInfo; // 무슨맵에 몇 마리 스폰시키는지
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MonsterData, stats, templateID, extraCellSize, isAggressive, offsetY, offsetX, yExtension, xExtension, spawnInfo)