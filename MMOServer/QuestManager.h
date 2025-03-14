#pragma once
#include <vector>
#include "MMO_enum.pb.h"
#include "S_QuestData.h"

class Quest;
class QuestManager
{
public:
	static void init();
private:
	static std::vector<S_QuestData> s_data;

public:
	QuestManager();
	~QuestManager();
	void report(protocol::mmo::E_QuestTaskType type, const std::string& what, int delta);
private:

	std::vector<Quest*> _quest;
};