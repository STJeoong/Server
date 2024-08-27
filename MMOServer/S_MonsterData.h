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

	// ������
	bool isAggressive;
	int offsetY;
	int offsetX;
	int yExtension;
	int xExtension;

	std::vector<std::pair<std::string, int>> spawnInfo; // �����ʿ� �� ���� ������Ű����
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MonsterData, stats, templateID, extraCellSize, isAggressive, offsetY, offsetX, yExtension, xExtension, spawnInfo)