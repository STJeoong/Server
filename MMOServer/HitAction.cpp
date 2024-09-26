#include "HitAction.h"
#include "GameObject.h"
#include "I_Targetable.h"

#pragma region public static
void HitAction::action(const S_SkillAction& skillActionDetail, const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
    int damage = targetActionDetail.referenceValue;
    if (targetActionDetail.isProportional)
    {
        // TODO : ��� ������
    }
    I_Targetable* target = reinterpret_cast<I_Targetable*>(targetObj);
    target->takeDamage(targetActionDetail.targetStats, damage);
}
#pragma endregion