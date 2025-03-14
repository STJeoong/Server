#include "PersistentChangeStats.h"
#include "I_Targetable.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Game.h"

using namespace protocol::mmo;
#pragma region public
void PersistentChangeStats::apply(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user)
{
	_target = dynamic_cast<I_Targetable*>(targetObj);
	_user = user;
	_duration = targetActionDetail.duration;
	_remainingTime = _duration;
	_interval = targetActionDetail.interval;
	_nextHitTime = _interval;
	_isProportional = targetActionDetail.isProportional;
	_isTargetStandard = targetActionDetail.isTargetStandard;
	_delta = targetActionDetail.deltaStats;
	_refStats = targetActionDetail.standardStats;

	if (_isProportional)
	{
		// TODO : 비례 데미지
	}
}
void PersistentChangeStats::revert()
{
	_target->removePersistentChangeStats(this);
}
#pragma endregion

#pragma region private
void PersistentChangeStats::invoke(const E_GameObjectEvent& evt, void* arg)
{
	switch (evt)
	{
	case E_GameObjectEvent::UPDATE: this->update(); break;
	case E_GameObjectEvent::INACTIVE:
	case E_GameObjectEvent::DESTROY: if (_duration != 0) this->revert(); break; // duration == 0 => 무제한
	}
}
void PersistentChangeStats::update()
{
	if (!_user->activeInHierarchy())
		_user = nullptr;
	// 만약 퍼뎀이라 해도 매 update마다 deltaStats을 update해줄 필요 없음. ( user가 도중에 disconnect할 수 있으니까 )
	_nextHitTime -= Game::UPDATE_DELTA_TIME;
	_remainingTime -= Game::UPDATE_DELTA_TIME;
	if (_nextHitTime <= 0)
	{
		this->action();
		_nextHitTime = _interval;
	}
	if (_remainingTime <= 0)
		this->revert();
}
void PersistentChangeStats::action()
{
	_target->changeStats(_delta, _user);
}
#pragma endregion