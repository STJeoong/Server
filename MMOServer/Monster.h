#pragma once
#include <queue>
#include "GameObject.h"
#include "S_Stats.h"
#include "MMO_struct.pb.h"
#include "S_MonsterData.h"
class Area;
class Monster : public GameObject
{
	friend class Map;
public:
	static void init();
private:
	static void spawn(const S_MonsterData& data);
	static void respawn();

	static std::queue<Monster*> s_deadMonsters;

public:
	virtual E_ObjectType objectType() const override { return E_ObjectType::MONSTER; }
	void broadcast(protocol::mmo::E_PacketID packetID);
	void broadcast(protocol::mmo::E_PacketID packetID, google::protobuf::Message& message);
	const S_Stats& stats() const { return _stats; }
	void stats(const S_Stats& val);
protected:
	Monster() = delete;
	Monster(const Monster&) = delete;
	Monster(Monster* copy) : GameObject(copy) {} // TODO : 복사할거 있으면 복사
	Monster(bool isActive, GameObject* parent) : GameObject(isActive, parent) {}
	Monster(bool isActive, Monster* copy, GameObject* parent) : GameObject(isActive, copy, parent) {}
	virtual ~Monster() = default; // TODO : 삭제할거 있으면 삭제
private:
	virtual GameObject* clone() override { return new Monster(this); }


	Area* _objArea = nullptr;
	S_Stats _stats = {};
	protocol::mmo::TransformInt _initTF; // 초기위치
};