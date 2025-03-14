#include "QuestTask.h"

using namespace protocol::mmo;
#pragma region public
QuestTask::QuestTask(const S_QuestTaskData& data)
{
	_type = data.type;
	_what = data.what;
	_goal = data.cnt;
}
void QuestTask::report(protocol::mmo::E_QuestTaskType type, const std::string& what, int delta)
{
	if (_type != type || _what != what)
		return;

	_current += delta;
}
#pragma endregion

#pragma region private
#pragma endregion