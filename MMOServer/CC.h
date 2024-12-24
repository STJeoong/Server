#pragma once
#include "S_SkillData.h"
#include "I_Revertable.h"
class I_Targetable;
class CC : public I_Revertable
{
public:
	void apply(const S_TargetBasedAction& targetActionDetail, I_Targetable* target);
	virtual void revert() override;
};