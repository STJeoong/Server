#pragma once
#include "I_Skill.h"

// ��߽�ų (�Ѽ����� ��ȿ)
class InstantSkill : public I_Skill
{
public:
	virtual void use(S_Stats& userStat, Map* map, TransformInt& pivot) override;
private:
};