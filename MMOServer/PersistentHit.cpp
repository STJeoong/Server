#include "PersistentHit.h"
#include "I_Targetable.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Game.h"

using namespace protocol::mmo;
#pragma region public
void PersistentHit::apply(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
	_target = reinterpret_cast<I_Targetable*>(targetObj);
	_duration = targetActionDetail.duration;
	_remainingTime = _duration;
	_hitInterval = targetActionDetail.hitInterval;
	_nextHitTime = _hitInterval;
	_isProportional = targetActionDetail.isProportional;
	_isTargetStandard = targetActionDetail.isTargetStandard;
	_refValue = targetActionDetail.referenceValue;
	_refStats = targetActionDetail.standardStats;
	_targetStats = targetActionDetail.targetStats;

	_damage = _refValue;
	if (_isProportional)
	{
		// TODO : 비례 데미지
	}
}
void PersistentHit::revert()
{
	_target->removePersistentHit(this);
	ObjectPool::release<PersistentHit>(this);
}
#pragma endregion

#pragma region private
void PersistentHit::invoke(const E_GameObjectEvent& evt, void* arg)
{
	switch (evt)
	{
	case E_GameObjectEvent::UPDATE: this->update(); break;
	case E_GameObjectEvent::INACTIVE:
	case E_GameObjectEvent::DESTROY: if (_duration != 0) this->revert(); break; // duration == 0 => 무제한
	}
}
void PersistentHit::update()
{
	_nextHitTime -= Game::UPDATE_DELTA_TIME;
	_remainingTime -= Game::UPDATE_DELTA_TIME;
	if (_nextHitTime <= 0)
	{
		this->action();
		_nextHitTime = _hitInterval;
	}
	if (_remainingTime <= 0)
		this->revert();
}
void PersistentHit::action()
{
	_target->takeDamage(_targetStats, _damage);
}
#pragma endregion