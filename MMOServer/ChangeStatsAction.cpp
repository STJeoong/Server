#include "ChangeStatsAction.h"
#include "GameObject.h"
#include "I_Targetable.h"

using namespace protocol::mmo;

#pragma region public static
I_Revertable* ChangeStatsAction::action(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
    S_Stats delta = targetActionDetail.deltaStats;
    if (targetActionDetail.isProportional)
    {
        // TODO : ºñ·Ê µ¥¹ÌÁö
    }
    I_Targetable* target = dynamic_cast<I_Targetable*>(targetObj);
    target->changeStats(delta);
    return nullptr;
}
#pragma endregion