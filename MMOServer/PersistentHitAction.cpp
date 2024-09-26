#include "PersistentHitAction.h"
#include "PersistentHit.h"
#include "GameObject.h"
#include "Area.h"
#include <ObjectPool.h>
#include "I_Targetable.h"

using namespace protocol::mmo;
#pragma region public
void PersistentHitAction::action(const S_SkillAction& skillActionDetail, const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
    PersistentHit* persistentHit = ObjectPool::get<PersistentHit>();
    I_Targetable* target = reinterpret_cast<I_Targetable*>(targetObj);
    persistentHit->apply(targetActionDetail, targetObj, user);
    target->addPersistentHit(persistentHit);
    if (skillActionDetail.skillType == E_SkillType::PERSISTENT_TARGET_BASED && skillActionDetail.revertOnExit)
    {
        Area* area = skillActionDetail.skillArea;
        std::function<void(Area&)> onAreaExit = [persistentHit, targetObj](Area& other) { if (other.gameObject() == targetObj) persistentHit->revert(); };
        area->addListenerOnAreaExit(onAreaExit);
    }
}
#pragma endregion

#pragma region private
#pragma endregion