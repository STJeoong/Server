#include "PersistentChangeStatsAction.h"
#include "PersistentChangeStats.h"
#include "GameObject.h"
#include "Area.h"
#include <ObjectPool.h>
#include "I_Targetable.h"

using namespace protocol::mmo;
#pragma region public
I_Revertable* PersistentChangeStatsAction::action(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
    PersistentChangeStats* persistent = ObjectPool::get<PersistentChangeStats>();
    I_Targetable* target = reinterpret_cast<I_Targetable*>(targetObj);
    persistent->apply(targetActionDetail, targetObj, user);
    target->addPersistentChangeStats(persistent);
    return persistent;
}
#pragma endregion

#pragma region private
#pragma endregion