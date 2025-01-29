#pragma once
#include "S_SkillData.h"
class GameObject;
class I_Revertable;
class PersistentChangeStatsAction
{
public:
	static I_Revertable* action(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user);
};

