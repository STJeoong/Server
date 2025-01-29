#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "MMO_protocol.pb.h"
#include "S_RectDefine.h"
#include "S_Stats.h"

class Area;
struct S_TargetBasedAction
{
	protocol::mmo::E_TargetBasedActionType actionType;
	int interval;
	int duration;
	protocol::mmo::E_CCType ccType;
	bool isProportional;
	bool isTargetStandard;
	S_Stats deltaStats;
	protocol::mmo::E_Stats standardStats;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_TargetBasedAction, actionType, interval, duration, ccType, isProportional, isTargetStandard,
	deltaStats, standardStats)


struct S_SkillAction
{
	protocol::mmo::E_SkillType skillType;
	S_RectDefine areaDefine;
	bool fixedRotation;

	// persistent
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