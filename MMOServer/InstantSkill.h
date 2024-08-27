#pragma once
#include "I_Skill.h"

// 즉발스킬 (한순간만 유효)
class InstantSkill : public I_Skill
{
public:
	virtual void use(S_Stats& userStat, Map* map, TransformInt& pivot) override;
private:
};