#pragma once
#include "Behaviour.h"

// your class must inherit this class.
// what you can override is below.
// void start()
// void update() 
// void onEnable()
// void onDisable()
// void onDestroy()
// void onActiveGameObject()
// void onInactiveGameObject()
// void onCollisionEnter(const Collision2D& collision)
// void onCollisionStay(const Collision2D& collision)
// void onCollisionExit(const Collision2D& collision)
// void onTriggerEnter(const Collider2D& other)
// void onTriggerStay(const Collider2D& other)
// void onTriggerExit(const Collider2D& other)
class MonoBehaviour : public Behaviour
{
public:
protected:
	MonoBehaviour() = default;
	virtual ~MonoBehaviour() = default;
	virtual bool onAddComponent(Component* component) override final { return true; }
	virtual void onRemoveComponent(Component* component) override final {}
	virtual void onEnableComponent(Component* component) override final { if (component == this) this->onEnable(); }
	virtual void onDisableComponent(Component* component) override final { if (component == this) this->onDisable(); }
	virtual void onApplyReservation() override final {}
	virtual void onMove() override final {}

	virtual void onEnable() {}
	virtual void onDisable() {}
};