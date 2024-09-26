#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>
#include "S_SkillData.h"
#include "MMO_struct.pb.h"
#include "S_Stats.h"
#include "S_RectDefine.h"

using namespace protocol::mmo;
class Map;
class GameObject;
class Skill
{
public:
	static void init();
	static Skill* get(const std::string& name) { return s_skillsByName[name]; }
	static Skill* get(int templateID) { return s_skillsByID[templateID]; }
private:
	static std::unordered_map<std::string, Skill*> s_skillsByName;
	static std::unordered_map<int, Skill*> s_skillsByID;


public:
	void use(GameObject* user);
	long long coolTime() const;
	long long delayTime() const { return _delayTime; }
	int mpUsed() const { return _mpUsed; }
private:
	Skill(const S_SkillData& config);
	~Skill() = default;

	S_SkillData _config = {};
	long long _coolTime = 0;
	long long _delayTime = 0;
	int _mpUsed = 0;
	void (*_action)(S_SkillAction& detail, GameObject* pivotObj, GameObject* user) = nullptr;
};