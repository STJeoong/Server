#pragma once
#include "I_Skill.h"

// 일정시간 간격으로 알림줌
// 지속스킬
class ContinuousSkill : public I_Skill
{
public:
	ContinuousSkill(int validTime) : _validTime(validTime) {}
	virtual void use(S_Stat& userStat, Map* map, TransformInt& pivot) override;
private:
	
	int _validTime = 0; // 스킬 유효시간
	int _interval = 0;
};