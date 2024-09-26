#include "Skill.h"
#include <vector>
#include <fstream>
#include "S_SkillData.h"
#include "InstantLocationBasedAction.h"
#include "InstantTargetBasedAction.h"
#include "PersistentLocationBasedAction.h"
#include "PersistentTargetBasedAction.h"

using json = nlohmann::json;
using namespace protocol::mmo;
#pragma region public static
void Skill::init()
{
	std::cout << "Skill init...\n";
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/SkillData.json");
	std::vector<S_SkillData> data;
	json j = json::parse(fstream);
	j.get_to(data);

	for (int i = 0; i < data.size(); ++i)
	{
		Skill* skill = new Skill(data[i]);
		s_skillsByID[data[i].templateID] = skill;
		s_skillsByName[data[i].skillName] = skill;
	}
}
#pragma endregion

#pragma region private static
#pragma endregion




#pragma region public
void Skill::use(GameObject* user) { _action(_config.skillAction, user, user); }
long long Skill::coolTime() const
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() + _coolTime + _delayTime;
}
#pragma endregion

#pragma region private
Skill::Skill(const S_SkillData& config) : _config(config)
{
	// TODO : Á¶¸³
	_mpUsed = config.mpUsed;
	_coolTime = config.coolTime;
	_delayTime = config.delayTime;

	const S_SkillAction& detail = config.skillAction;
	switch (detail.skillType)
	{
	case E_SkillType::INSTANT_LOCATION_BASED: _action = InstantLocationBasedAction::action; break;
	case E_SkillType::INSTANT_TARGET_BASED: _action = InstantTargetBasedAction::action; break;
	case E_SkillType::PERSISTENT_LOCATION_BASED: _action = PersistentLocationBasedAction::action; break;
	case E_SkillType::PERSISTENT_TARGET_BASED: _action = PersistentTargetBasedAction::action; break;
	}
}
#pragma endregion

std::unordered_map<std::string, Skill*> Skill::s_skillsByName;
std::unordered_map<int, Skill*> Skill::s_skillsByID;