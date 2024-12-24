#include "InstantTargetBasedAction.h"
#include "GameObject.h"
#include "HitAction.h"
#include "CCAction.h"
#include "BuffAction.h"
#include "PersistentHitAction.h"
#include "Utils.h"
#include "Map.h"

using namespace protocol::mmo;
#pragma region public
void InstantTargetBasedAction::action(S_SkillAction& detail, GameObject* pivotObj, GameObject* user)
{
	Map* map = pivotObj->map();
	int cnt = 0;
	std::vector<GameObject*> list;

	AABB aabb = Utils::calcAABB(detail.areaDefine, pivotObj->transform(), detail.fixedRotation);
	map->overlapsBox(aabb, detail.filter, list);
	for (GameObject* targetObj : list)
	{
		if (!(detail.filter & ((int)targetObj->objectType())))
			continue;
		if (targetObj == user && !detail.containMe)
			continue;
		if (targetObj != user && detail.onlyMe)
			continue;

		for (const S_TargetBasedAction& targetAction : detail.targetBasedActions)
		{
			switch (targetAction.actionType)
			{
			case E_TargetBasedActionType::TARGET_BASED_ACTION_TYPE_HIT: HitAction::action(targetAction, targetObj, user); break;
			case E_TargetBasedActionType::TARGET_BASED_ACTION_TYPE_BUFF: BuffAction::action(targetAction, targetObj, user); break;
			case E_TargetBasedActionType::TARGET_BASED_ACTION_TYPE_CC: CCAction::action(targetAction, targetObj, user); break;
			case E_TargetBasedActionType::TARGET_BASED_ACTION_TYPE_PERSISTENT_HIT: PersistentHitAction::action(targetAction, targetObj, user); break;
			}
		}
		if (detail.maxTarget == 0)
			continue;
		++cnt;
		if (cnt == detail.maxTarget)
			break;
	}
}
#pragma endregion

#pragma region private
#pragma endregion