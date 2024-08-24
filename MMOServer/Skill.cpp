#include "Skill.h"
#include "S_SkillData.h"
#include <vector>
#include <fstream>
#include "I_Skill.h"

using json = nlohmann::json;
#pragma region public static
void Skill::init()
{
	// TODO
	/*std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/SkillData.json");
	std::vector<S_SkillData> data;
	json j = json::parse(fstream);
	j.get_to(data);

	for (int i = 0; i < data.size(); ++i)
		s_skills[data[i].name] = new Skill(data[i]);*/
}
#pragma endregion

#pragma region private static
#pragma endregion




#pragma region public
void Skill::use(S_Stat& userStat, Map* map, TransformInt& pivot)
{
	_skill->use(userStat, map, pivot);
}
long long Skill::coolTime() const
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() + _coolTime;
}
#pragma endregion

#pragma region private
Skill::Skill(const S_SkillData& config)
{
	// TODO : Á¶¸³
}
#pragma endregion

std::unordered_map<std::string, Skill*> Skill::s_skills;