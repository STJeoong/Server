#pragma once
#include "Behaviour.h"
class Area;
class Player;
class MonsterController : public Behaviour
{
	friend class GameObject;
public:
protected:
	MonsterController() = delete;
	MonsterController(const MonsterController&) = delete;
	~MonsterController() = default;
	MonsterController(GameObject* obj) : Behaviour(obj) {}
	virtual void update() override;
	virtual void onAreaEnter(Area& my, Area& other) override;
	virtual void onAreaExit(Area& my, Area& other) override;
private:
	virtual Component* createInstance(GameObject* obj) override { return new MonsterController(obj); }
	virtual void copyTo(Component* instance) override {}

	int _nextUpdateCnt = 0;
	Player* _target = nullptr;
	Area* _aggressiveArea = nullptr;
};