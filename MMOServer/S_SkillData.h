#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "MMO_protocol.pb.h"
#include "S_RectDefine.h"

class Area;
struct S_TargetBasedAction
{
	protocol::mmo::E_TargetBasedActionType actionType;
	int hitInterval;
	int duration;
	protocol::mmo::E_CCType ccType;
	bool isProportional;
	bool isTargetStandard;
	int referenceValue;
	protocol::mmo::E_Stats standardStats;
	protocol::mmo::E_Stats targetStats;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_TargetBasedAction, actionType, hitInterval, duration, ccType, isProportional, isTargetStandard,
	referenceValue,	standardStats, targetStats)


struct S_SkillAction
{
	protocol::mmo::E_SkillType skillType;
	S_RectDefine areaDefine;
	bool fixedRotation;

	// persistent
	Area* skillArea = nullptr;
	int areaDuration;
	int interval;
	bool revertOnExit;

	// location based skill
	protocol::mmo::E_ObjectType objectType;
	int templateID;

	// target based skill
	int filter;
	bool containMe;
	bool onlyMe;
	int maxTarget;
	std::vector<S_TargetBasedAction> targetBasedActions;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_SkillAction, skillType, areaDefine, fixedRotation, areaDuration, interval, revertOnExit,
	objectType, templateID,	filter, containMe, onlyMe, maxTarget, targetBasedActions)


struct S_SkillData
{
	std::string skillName;
	int templateID;
	int coolTime;
	int delayTime;
	int mpUsed;
	S_SkillAction skillAction;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_SkillData, skillName, templateID, coolTime, delayTime, mpUsed, skillAction)