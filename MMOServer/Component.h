#pragma once
#include "E_GameObjectEvent.h"
#include "MMO_struct.pb.h"
using namespace protocol::mmo;
class GameObject;
class Area;
class Component
{
	friend class GameObject;
	friend class Game;
public:
	const GameObject* gameObject() const { return _gameObject; }
	GameObject* gameObject() { return _gameObject; }
protected:
	Component() = default;
	virtual ~Component() = default;

	void invokeAll(const E_GameObjectEvent& evt, void* arg);
	virtual void onUpdate() {}
	virtual void onActiveGameObject() {}
	virtual void onInactiveGameObject() {}
	virtual void onDestroy() {} // ���� �غ��϶�� ����. ���� ������ Game::update�� �� �������� �̷����
	virtual void onAreaEnter(Area& my, Area& other) {}
	virtual void onAreaStay(Area& my, Area& other) {}
	virtual void onAreaExit(Area& my, Area& other) {}
private:
	void invoke(const E_GameObjectEvent& evt, void* arg);
	void gameObject(GameObject* obj) { _gameObject = obj; }


	GameObject* _gameObject = nullptr;
};