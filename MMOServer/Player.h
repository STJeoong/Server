#pragma once
#include <vector>
#include "GameObject.h"
#include "MMO_protocol.pb.h"
#include "S_PlayerData.h"
#include "S_Stats.h"
#include "I_Targetable.h"

class Area;
class PlayerController;
class Buff;
class CC;
class PersistentChangeStats;
class Equipment;
class Consume;
class Player : public GameObject, public I_Targetable
{
	friend class Map;

	static const int MAX_INVEN_SIZE = 20;
public:
	static void init();
private:
	static void onConnect(int serial);
	static void onDisconnect(int serial);
	static void onEnterReq(int serial);
	static void onMoveReq(int serial, const protocol::mmo::Move_Req& req);
	static void onIdleReq(int serial);
	static void onNormalAttackReq(int serial);

	static std::vector<Player*> s_players;
	static std::vector<S_PlayerData> s_playerData;
public:
	virtual protocol::mmo::E_ObjectType objectType() const override { return protocol::mmo::E_ObjectType::PLAYER; }
	int networkSerial() const { return _networkSerial; }
	void broadcastPacket(protocol::mmo::E_PacketID packetID, bool includeMe = false);
	void broadcastPacket(protocol::mmo::E_PacketID packetID, google::protobuf::Message& message, bool includeMe = false);

	// I_Targetable을(를) 통해 상속됨
	virtual void addBuff(Buff* buff) override;
	virtual void removeBuff(Buff* buff) override;
	virtual void addCC(CC* cc) override;
	virtual void removeCC(CC* cc) override;
	virtual void addPersistentChangeStats(PersistentChangeStats* persistent) override;
	virtual void removePersistentChangeStats(PersistentChangeStats* persistent) override;
	virtual void changeStats(S_Stats delta) override;
protected:
	Player() = delete;
	Player(const Player&) = delete;
	Player(Player* copy) : GameObject(copy) {} // TODO 복사할 거 있으면 복사
	Player(bool isActive, GameObject* parent) : GameObject(isActive, parent) {}
	Player(bool isActive, Player* copy, GameObject* parent) : GameObject(isActive, copy, parent) {}
	virtual ~Player() = default; // TODO : 삭제할거 있으면 삭제

private:
	virtual GameObject* clone() override { return new Player(this); }

	int _networkSerial = -1;
	Area* _objArea = nullptr;
	PlayerController* _controller = nullptr;
	S_Stats _stats = {};

	std::vector<Buff*> _buff;
	std::vector<CC*> _cc;
	std::vector<PersistentChangeStats*> _persistent;

	// Inventory
	Equipment* _equipments[Player::MAX_INVEN_SIZE] = {};
	Consume* _consume[Player::MAX_INVEN_SIZE] = {};
};

// id == -1 => disconnected 상태
// id != -1 && _state == NONE => connected 상태지만 entergame은 아닌상태
// id != -1 && _state != NONE => entergame 상태