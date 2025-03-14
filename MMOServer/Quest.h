#pragma once
#include <vector>
#include "MMO_enum.pb.h"
#include "E_QuestState.h"
#include "S_QuestData.h"

class QuestTask;
class Quest
{
public:
	Quest(const S_QuestData& data);
	~Quest();
	E_QuestState state() const { return _state; }
	void report(protocol::mmo::E_QuestTaskType type, const std::string& what, int delta);
private:

	E_QuestState _state = E_QuestState::PROGRESSABLE;
	std::vector<QuestTask*> _tasks;
};