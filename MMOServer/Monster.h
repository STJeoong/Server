#pragma once
#include <queue>
#include "GameObject.h"
#include "S_Stats.h"
#include "MMO_struct.pb.h"
#include "S_MonsterData.h"
#include "I_Targetable.h"
class Area;
class Buff;
class CC;
class PersistentChangeStats;
class Monster : public GameObject, public I_Targetable
{
	friend class Map;
public:
	static void init();
	static const S_MonsterData& monsterData(int templateID) { return s_monsterData[templateID]; }
private:
	static void spawn(const S_MonsterData& data);
	static void respawn();

	static std::queue<Monster*> s_deadMonsters;
	static std::vector<S_MonsterData> s_monsterData;

public:
	virtual protocol::mmo::E_ObjectType objectType() const override { return protocol::mmo::E_ObjectType::MONSTER; }
	void broadcastPacket(protocol::mmo::E_PacketID packetID);
	void broadcastPacket(protocol::mmo::E_PacketID packetID, google::protobuf::Message& message);
	const S_Stats& stats() const { return _stats; }

	// I_Targetable을(를) 통해 상속됨
	virtual void addBuff(Buff* buff) override;
	virtual void removeBuff(Buff* buff) override;
	virtual void addCC(CC* cc) override;
	virtual void removeCC(CC* cc) override;
	virtual void addPersistentChangeStats(PersistentChangeStats* persistent) override;
	virtual void removePersistentChangeStats(PersistentChangeStats* persistent) override;
	virtual void changeStats(S_Stats delta) override;
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
	protocol::mmo::TransformInt _initTF; // 초기위치 (리스폰할때 사용)

	std::vector<Buff*> _buff;
	std::vector<CC*> _cc;
	std::vector<PersistentChangeStats*> _persistent;
};