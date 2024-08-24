#pragma once
#include "I_Skill.h"

// �����ð� �������� �˸���
// ���ӽ�ų
class ContinuousSkill : public I_Skill
{
public:
	ContinuousSkill(int validTime) : _validTime(validTime) {}
	virtual void use(S_Stat& userStat, Map* map, TransformInt& pivot) override;
private:
	
	int _validTime = 0; // ��ų ��ȿ�ð�
	int _interval = 0;
};