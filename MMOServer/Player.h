#pragma once
#include <vector>
#include "GameObject.h"
#include "MMO_protocol.pb.h"

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

	static std::vector<Player*> s_players;
	
public:
	virtual E_ObjectType objectType() const override { return E_ObjectType::PLAYER; }
protected:
	Player() = delete;
	Player(const Player&) = delete;
	Player(Player* copy) : GameObject(copy) {} // TODO ������ �� ������ ����
	Player(bool isActive, GameObject* parent) : GameObject(isActive, parent) {}
	Player(bool isActive, Player* copy, GameObject* parent) : GameObject(isActive, copy, parent) {}
	virtual ~Player() = default; // TODO : �����Ұ� ������ ����

private:
	virtual GameObject* clone() override { return new Player(this); }

	PlayerController* _controller = nullptr;
};

// id == -1 => disconnected ����
// id != -1 && _state == NONE => connected �������� entergame�� �ƴѻ���
// id != -1 && _state != NONE => entergame ����