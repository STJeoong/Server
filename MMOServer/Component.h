#pragma once
#include "E_GameObjectEvent.h"

class GameObject;
class Area;
class Component
{
	friend class GameObject;
	friend class Map;
public:
	const GameObject* gameObject() const { return _gameObject; }
	GameObject* gameObject() { return _gameObject; }
protected:
	Component() = delete;
	Component(GameObject* obj) : _gameObject(obj) {}
	virtual ~Component() = default;

	void invokeAll(const E_GameObjectEvent& evt, void* arg);
	virtual void awake() {}
	virtual void onUpdate() {}
	virtual void onActiveGameObject() {}
	virtual void onInactiveGameObject() {}
	virtual void onDestroy() {} // ���� �غ��϶�� ����. ���� ������ Game::update�� �� �������� �̷����
	virtual void onAreaEnter(Area& my, Area& other) {}
	virtual void onAreaStay(Area& my, Area& other) {}
	virtual void onAreaExit(Area& my, Area& other) {}

	GameObject* const _gameObject = nullptr;
private:
	void invoke(const E_GameObjectEvent& evt, void* arg);
	virtual Component* createInstance(GameObject* obj) = 0;
	virtual void copyTo(Component* instance) = 0;
};