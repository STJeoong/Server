#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <queue>
#include "S_SkillData.h"
#include "MMO_struct.pb.h"
#include "S_Stats.h"

using namespace protocol::mmo;
class I_Skill;
class I_SkillUser;
class Map;
class Skill
{
public:
	static void init();
	static Skill* get(const std::string& name) { return s_skills[name]; }
private:
	static std::unordered_map<std::string, Skill*> s_skills;


public:
	void use(S_Stats& userStat, Map* map, TransformInt& pivot);
	long long coolTime() const;
private:
	Skill(const S_SkillData& config);
	~Skill() = default;

	long long _coolTime = 0;
	I_Skill* _skill = nullptr;
	std::queue<std::tuple<int, S_Stats&, Map*, TransformInt&>> _q;
	int _totalCnt = 0; // action�� �� ����ϴ���
	int _interval = 0; // �� ���� �ð� ������ �ΰ� action�� �ϴ���
};