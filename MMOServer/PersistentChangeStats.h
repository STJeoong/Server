#pragma once
#include "S_SkillData.h"
#include "MMO_protocol.pb.h"
#include "E_GameObjectEvent.h"
#include "I_Revertable.h"
#include "S_Stats.h"
class GameObject;
class I_Targetable;
class PersistentChangeStats : public I_Revertable
{
	friend class GameObject;
public:
	void apply(const S_TargetBasedAction& targetActionDetail, GameObject* targetObj, GameObject* user);
	// I_Revertable을(를) 통해 상속됨
	virtual void revert() override;
private:
	void invoke(const E_GameObjectEvent& evt, void* arg);
	void update();
	void action();

	I_Targetable* _target = nullptr;
	int _duration = 0;
	int _remainingTime = 0; // 남은 시간
	int _interval = 0;
	int _nextHitTime = 0;

	bool _isProportional;
	bool _isTargetStandard;
	S_Stats _delta;
	protocol::mmo::E_Stats _refStats;
};