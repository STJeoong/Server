#pragma once
#include <vector>
#include "MMO_protocol.pb.h"
#include "Behaviour.h"
using namespace protocol::mmo;
class Area;
class Map;
class Player : public Behaviour
{
	friend class GameObject;
public:
	static void init();
private:
	static Player* createPlayer();
	static void onConnect(int serial);
	static void onDisconnect(int serial);
	static void onEnterReq(int serial);
	static void onMoveReq(int serial, const Move_Req& req);

	static std::vector<Player*> s_players;
	
public:
protected:
	Player() = delete;
	Player(const Player&) = delete;
	Player(GameObject* obj) : Behaviour(obj) {}
	~Player() = default;
	virtual void onAreaEnter(Area& my, Area& other) override;
	virtual void onAreaExit(Area& my, Area& other) override;
private:
	void move(const Move_Req& req);
	virtual Component* createInstance(GameObject* obj) override { return new Player(obj); }
	virtual void copyTo(Component* instance) override {}

	
	Area* _aoi = nullptr;
	Area* _myObjArea = nullptr;
	Map* _map = nullptr; // ���� ��ġ�� ��
};

// id == -1 => disconnected ����
// id != -1 && _state == NONE => connected �������� entergame�� �ƴѻ���
// id != -1 && _state != NONE => entergame ����