#pragma once
#include <vector>
#include "GameObject.h"
#include "MMO_protocol.pb.h"
#include "S_Stats.h"

class Area;
class PlayerController;
class Player : public GameObject
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
	virtual E_ObjectType objectType() const override { return E_ObjectType::PLAYER; }
	void broadcast(protocol::mmo::E_PacketID packetID, bool includeMe = true);
	void broadcast(protocol::mmo::E_PacketID packetID, google::protobuf::Message& message, bool includeMe = true);
protected:
	Player() = delete;
	Player(const Player&) = delete;
	Player(Player* copy) : GameObject(copy) {} // TODO 복사할 거 있으면 복사
	Player(bool isActive, GameObject* parent) : GameObject(isActive, parent) {}
	Player(bool isActive, Player* copy, GameObject* parent) : GameObject(isActive, copy, parent) {}
	virtual ~Player() = default; // TODO : 삭제할거 있으면 삭제

private:
	virtual GameObject* clone() override { return new Player(this); }

	Area* _objArea = nullptr;
	PlayerController* _controller = nullptr;
	S_Stats _stats = {};
};

// id == -1 => disconnected 상태
// id != -1 && _state == NONE => connected 상태지만 entergame은 아닌상태
// id != -1 && _state != NONE => entergame 상태