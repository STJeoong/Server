#pragma once
#include "Behaviour.h"
#include "MMO_protocol.pb.h"
class Area;
class Player;
class Monster;
class MonsterController : public Behaviour
{
	friend class GameObject;
public:
	void aggressiveArea(Area* area) { _aggressiveArea = area; }
	void normalAttackArea(Area* area) { _normalAttackArea = area; }
protected:
	MonsterController() = delete;
	MonsterController(const MonsterController&) = delete;
	~MonsterController() = default;
	MonsterController(GameObject* obj) : Behaviour(obj) {}
	virtual void awake() override { _me = reinterpret_cast<Monster*>(_gameObject); }
	virtual void update() override;
	virtual void onAreaEnter(Area& my, Area& other) override;
private:
	void idle();
	void move();
	void attack();
	void decisionWhenNoTarget();
	void decisionWhenTargetExist(); // Å¸°Ù ÃßÀû
	Player* findNewTarget();
	void move(protocol::mmo::E_Dir dir);
	virtual Component* createInstance(GameObject* obj) override { return new MonsterController(obj); }
	virtual void copyTo(Component* instance) override {}

	Monster* _me = nullptr;
	int _nextUpdateCnt = 0;
	Player* _target = nullptr;
	Area* _aggressiveArea = nullptr;
	Area* _normalAttackArea = nullptr;
};