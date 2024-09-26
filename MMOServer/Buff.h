#pragma once
#include "S_SkillData.h"
class I_Targetable;
class Buff
{
public:
	void apply(const S_TargetBasedAction& targetActionDetail, I_Targetable* target);
	void revert();
};