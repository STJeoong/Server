#include "PersistentHitAction.h"
#include "PersistentHit.h"
#include "GameObject.h"
#include "Area.h"
#include <ObjectPool.h>
#include "I_Targetable.h"

using namespace protocol::mmo;
#pragma region public
I_Revertable* PersistentHitAction::action(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
    PersistentHit* persistentHit = ObjectPool::get<PersistentHit>();
    I_Targetable* target = reinterpret_cast<I_Targetable*>(targetObj);
    persistentHit->apply(targetActionDetail, targetObj, user);
    target->addPersistentHit(persistentHit);
    return persistentHit;
}
#pragma endregion

#pragma region private
#pragma endregion