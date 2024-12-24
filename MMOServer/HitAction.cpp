#include "HitAction.h"
#include "GameObject.h"
#include "I_Targetable.h"

#pragma region public static
I_Revertable* HitAction::action(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
    int damage = targetActionDetail.referenceValue;
    if (targetActionDetail.isProportional)
    {
        // TODO : ºñ·Ê µ¥¹ÌÁö
    }
    I_Targetable* target = reinterpret_cast<I_Targetable*>(targetObj);
    target->takeDamage(targetActionDetail.targetStats, damage);
    return nullptr;
}
#pragma endregion