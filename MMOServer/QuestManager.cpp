#include "QuestManager.h"
#include "Quest.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
#pragma region public static
void QuestManager::init()
{
	std::cout << "Quest init...\n";
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/QuestData.json");
	json j = json::parse(fstream);
	j.get_to(s_data);
}
#pragma endregion


#pragma region public
QuestManager::QuestManager()
{
	for (const S_QuestData& data : s_data)
		_quest.push_back(new Quest(data));
}
QuestManager::~QuestManager()
{
	for (Quest* q : _quest)
		delete q;
}
void QuestManager::report(protocol::mmo::E_QuestTaskType type, const std::string& what, int delta)
{
	for (Quest* q : _quest)
		if (q->state() == E_QuestState::COMPLETABLE || q->state() == E_QuestState::IN_PROGRESS)
			q->report(type, what, delta);
}
#pragma endregion

#pragma region private
#pragma endregion

std::vector<S_QuestData> QuestManager::s_data = {};