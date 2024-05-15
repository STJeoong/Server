#pragma once
#include "E_GameObjectEvent.h"
class GameObject;
class Collision2D;
class Collider2D;
// all components have to restrict their constructor and destructor's authority
// and make GameObject class your friend class.
// because it must be created or destroyed only in GameObject.
class Component
{
	friend class GameObject;
public:
	const GameObject* gameObject() const { return _gameObject; }
	GameObject* gameObject() { return _gameObject; }
protected:
	Component() = default;
	virtual ~Component() = default;
	// used to invoke all component's invoke() in gameobject
	// this is made for inner component. if caller was component, you can use this.
	// only component can use this when they want to trigger event
	void invokeAll(const E_GameObjectEvent& evt, void* arg);
	virtual void onUpdate() {}
	virtual void onDestroy() {}
	virtual void onActiveGameObject() {}
	virtual void onInactiveGameObject() {}
	virtual void onCollisionEnter(const Collision2D& collision) {}
	virtual void onCollisionStay(const Collision2D& collision) {}
	virtual void onCollisionExit(const Collision2D& collision) {}
	virtual void onTriggerEnter(const Collider2D& other) {}
	virtual void onTriggerStay(const Collider2D& other) {}
	virtual void onTriggerExit(const Collider2D& other) {}
	virtual bool onAddComponent(Component* component) { return true; }
	virtual void onRemoveComponent(Component* component) {}
	virtual void onEnableComponent(Component* component) {}
	virtual void onDisableComponent(Component* component) {}
	virtual void onApplyReservation() {}
	virtual void onMove() {} // when gameObject is moved without physics simulation using transform() function, this function is called.
private:
	virtual Component* clone() = 0;
	void setGameObject(GameObject* obj) { _gameObject = obj; }
	// used to invoke just one component's invoke()
	bool invoke(const E_GameObjectEvent& evt, void* arg);

	GameObject* _gameObject = nullptr;
};

