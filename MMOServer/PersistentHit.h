#pragma once
#include "S_SkillData.h"
#include "MMO_protocol.pb.h"
#include "E_GameObjectEvent.h"
class GameObject;
class I_Targetable;
class PersistentHit
{
	friend class GameObject;
public:
	void apply(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user);
	void revert();
private:
	void invoke(const E_GameObjectEvent& evt, void* arg);
	void update();
	void action();

	I_Targetable* _target = nullptr;
	int _duration = 0;
	int _remainingTime = 0; // 남은 시간
	int _hitInterval = 0;
	int _nextHitTime = 0;

	bool _isProportional;
	bool _isTargetStandard;
	int _refValue;
	protocol::mmo::E_Stats _refStats;
	protocol::mmo::E_Stats _targetStats;
	int _damage;
};