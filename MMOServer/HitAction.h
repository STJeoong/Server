#pragma once
#include "S_SkillData.h"
class GameObject;
class HitAction
{
public:
	static void action(const S_SkillAction& skillActionDetail, const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user);
};