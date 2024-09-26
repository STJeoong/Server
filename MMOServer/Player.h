#pragma once
#include <vector>
#include "GameObject.h"
#include "MMO_protocol.pb.h"
#include "S_Stats.h"
#include "I_Targetable.h"

class Area;
class PlayerController;
class Buff;
class CC;
class PersistentHit;
class Player : public GameObject, public I_Targetable
{
	friend class Map;
public:
	static void init();
private:
	static void onConnect(int serial);
	static void onDisconnect(int serial);
	static void onEnterReq(int serial);
	static void onMoveReq(int serial, const protocol::mmo::Move_Req& req);
	static void onIdleReq(int serial);

	static std::vector<Player*> s_players;
	
public:
	virtual protocol::mmo::E_ObjectType objectType() const override { return protocol::mmo::E_ObjectType::PLAYER; }
	int networkSerial() const { return _networkSerial; }
	void broadcastPacket(protocol::mmo::E_PacketID packetID, bool includeMe = true);
	void broadcastPacket(protocol::mmo::E_PacketID packetID, google::protobuf::Message& message, bool includeMe = true);

	// I_Targetable��(��) ���� ��ӵ�
	virtual void addBuff(Buff* buff) override;
	virtual void removeBuff(Buff* buff) override;
	virtual void addCC(CC* cc) override;
	virtual void removeCC(CC* cc) override;
	virtual void addPersistentHit(PersistentHit* persistentHit) override;
	virtual void removePersistentHit(PersistentHit* persistentHit) override;
	virtual void takeDamage(protocol::mmo::E_Stats what, int val) override;
protected:
	Player() = delete;
	Player(const Player&) = delete;
	Player(Player* copy) : GameObject(copy) {} // TODO ������ �� ������ ����
	Player(bool isActive, GameObject* parent) : GameObject(isActive, parent) {}
	Player(bool isActive, Player* copy, GameObject* parent) : GameObject(isActive, copy, parent) {}
	virtual ~Player() = default; // TODO : �����Ұ� ������ ����

private:
	virtual GameObject* clone() override { return new Player(this); }

	int _networkSerial = -1;
	Area* _objArea = nullptr;
	PlayerController* _controller = nullptr;
	S_Stats _stats = {};

	std::vector<Buff*> _buff;
	std::vector<CC*> _cc;
	std::vector<PersistentHit*> _persistentHit;
};

// id == -1 => disconnected ����
// id != -1 && _state == NONE => connected �������� entergame�� �ƴѻ���
// id != -1 && _state != NONE => entergame ����