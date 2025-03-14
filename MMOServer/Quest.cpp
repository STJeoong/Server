#include "Quest.h"
#include "QuestTask.h"

#pragma region public
Quest::Quest(const S_QuestData& data)
{
	for (const S_QuestTaskData& taskData : data.tasks)
		_tasks.push_back(new QuestTask(taskData));
}
Quest::~Quest()
{
	for (QuestTask* task : _tasks)
		delete task;
}
void Quest::report(protocol::mmo::E_QuestTaskType type, const std::string& what, int delta)
{
	for (QuestTask* task : _tasks)
		task->report(type, what, delta);

	for (QuestTask* task : _tasks)
		if (!task->isFinished())
		{
			_state = E_QuestState::IN_PROGRESS;
			return;
		}
	_state = E_QuestState::COMPLETABLE;
}
#pragma endregion

#pragma region private
#pragma endregion