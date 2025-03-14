#pragma once
#include <string>
#include "MMO_enum.pb.h"
#include "S_QuestData.h"

class QuestTask
{
public:
	QuestTask(const S_QuestTaskData& data);
	bool isFinished() const { return _current >= _goal; }
	void report(protocol::mmo::E_QuestTaskType type, const std::string& what, int delta);
private:

	protocol::mmo::E_QuestTaskType _type;
	std::string _what = 0;
	int _current = 0;
	int _goal = 0;
};