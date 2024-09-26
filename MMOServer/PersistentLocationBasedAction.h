#pragma once
#include "S_SkillData.h"
class GameObject;
class PersistentLocationBasedAction
{
public:
	static void action(S_SkillAction& detail, GameObject* pivotObj, GameObject* user);
};

