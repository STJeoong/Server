#pragma once
#include <nlohmann/json.hpp>
#include <tuple>
#include <string>
#include <vector>
#include <map>
#include "S_Stats.h"
#include "S_RectDefine.h"
#include "S_SkillData.h"
#include <basetsd.h>

struct S_SpawnInfo
{
	std::string mapName;
	int howMany;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_SpawnInfo, mapName, howMany)
struct S_MonsterNormalAttack
{
	bool onlyTarget;
	int maxTarget;
	S_RectDefine areaDefine;
	bool fixedRotation;
	int delayTime;
	std::vector<S_TargetBasedAction> actions;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MonsterNormalAttack, onlyTarget, maxTarget, areaDefine, fixedRotation, delayTime, actions)
struct S_MonsterData
{
	std::string monsterName;
	UINT8 templateID;
	int extraCellSize;
	S_Stats stats;

	S_MonsterNormalAttack normalAttack;
	std::vector<S_SpawnInfo> spawnInfo; // 무슨맵에 몇 마리 스폰시키는지
	std::map<std::string, int> equipDropInfo;
	std::map<std::string, int> consumeDropInfo;

	// 선공몹
	bool isAggressive;
	S_RectDefine aggressiveArea;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MonsterData, monsterName, templateID, extraCellSize, stats, normalAttack, spawnInfo, equipDropInfo, consumeDropInfo, isAggressive, aggressiveArea)