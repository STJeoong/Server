#include "PersistentTargetBasedAction.h"
#include "ChangeStatsAction.h"
#include "CCAction.h"
#include "BuffAction.h"
#include "PersistentChangeStatsAction.h"
#include "GameObject.h"
#include "Area.h"
#include "Rectangular.h"
#include "Map.h"
#include "I_Revertable.h"

using namespace protocol::mmo;
#pragma region public
void PersistentTargetBasedAction::action(S_SkillAction& detail, GameObject* pivotObj, GameObject* user)
{
	Area* area = pivotObj->addComponent<Area>();
	area->addShape(new Rectangular(area, detail.areaDefine));
	area->layer(E_Layer::SKILL);

	for (const S_TargetBasedAction& targetAction : detail.targetBasedActions)
	{
		I_Revertable* (*f)(const S_TargetBasedAction&, GameObject*, GameObject*) = nullptr;
		switch (targetAction.actionType)
		{
		case E_TargetBasedActionType::BUFF: f = BuffAction::action; break;
		case E_TargetBasedActionType::CROWD_CONTROL: f = CCAction::action; break;
		case E_TargetBasedActionType::PERSISTENT_CHANGE_STATS: f = PersistentChangeStatsAction::action; break;
		}

		std::function<void(Area&)> onAreaEnter = [f, area, detail, targetAction, user](Area& other)
		{
			GameObject* targetObj = other.gameObject();
			if (!(detail.filter & ((int)targetObj->objectType())))
				return;
			if (targetObj == user && !detail.containMe)
				return;
			if (targetObj != user && detail.onlyMe)
				return;

			I_Revertable* revertable = f(targetAction, targetObj, user);
			if (detail.revertOnExit)
			{
				std::function<void(Area&)> onAreaExit = [revertable, targetObj](Area& other) { if (other.gameObject() == targetObj) revertable->revert(); };
				area->addListenerOnAreaExit(onAreaExit);
			}
		};
		area->addListenerOnAreaEnter(onAreaEnter);
	}
	pivotObj->addTimer(detail.areaDuration, [area](GameObject* obj) { obj->map()->destroy(area); });
}
#pragma endregion

#pragma region private
#pragma endregion