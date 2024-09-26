#pragma once
#include "S_SkillData.h"
class GameObject;
class PersistentTargetBasedAction
{
public:
	static void action(S_SkillAction& detail, GameObject* pivotObj, GameObject* user);
};