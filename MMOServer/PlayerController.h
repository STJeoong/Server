#pragma once
#include <unordered_map>
#include "Behaviour.h"
#include "MMO_protocol.pb.h"

class Skill;
class Area;
class Player;
class PlayerController : public Behaviour
{
	friend class GameObject;
public:
	void myObjArea(Area* val) { _myObjArea = val; }
	void move(const protocol::mmo::Move_Req& req);
protected:
	PlayerController() = delete;
	PlayerController(const PlayerController&) = delete;
	PlayerController(GameObject* obj) : Behaviour(obj) {}
	~PlayerController() = default;
	virtual void awake() override { _me = reinterpret_cast<Player*>(_gameObject); }
	virtual void onAreaEnter(Area& my, Area& other) override;
	virtual void onAreaExit(Area& my, Area& other) override;
private:
	virtual Component* createInstance(GameObject* obj) override { return new PlayerController(obj); }
	virtual void copyTo(Component* instance) override {}

	Player* _me = nullptr;
	Area* _myObjArea = nullptr;
	Skill* _q = nullptr;
	Skill* _w = nullptr;
	Skill* _e = nullptr;
	Skill* _r = nullptr;
	std::unordered_map<int, long long> _coolTimes;
};