#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Collider2D.h"
#include "Collision2D.h"

#pragma region public
#pragma endregion

#pragma region protected
void Component::invokeAll(const E_GameObjectEvent& evt, void* arg)
{
	const std::vector<Component*>& components = this->gameObject()->components();
	for (int i = 0; i < components.size(); ++i)
		components[i]->invoke(evt, arg);
}
#pragma endregion

#pragma region private
bool Component::invoke(const E_GameObjectEvent& evt, void* arg)
{
	switch (evt)
	{
	case E_GameObjectEvent::ACTIVE: this->onActiveGameObject(); return true;
	case E_GameObjectEvent::INACTIVE: this->onInactiveGameObject(); return true;
	case E_GameObjectEvent::UPDATE: this->onUpdate(); return true;
	case E_GameObjectEvent::DESTROY: this->onDestroy(); return true;
	case E_GameObjectEvent::COLLISION_ENTER: { Collision2D* p = reinterpret_cast<Collision2D*>(arg); this->onCollisionEnter(*p); return true; }
	case E_GameObjectEvent::COLLISION_STAY: { Collision2D* p = reinterpret_cast<Collision2D*>(arg); this->onCollisionStay(*p); return true; }
	case E_GameObjectEvent::COLLISION_EXIT: { Collision2D* p = reinterpret_cast<Collision2D*>(arg); this->onCollisionExit(*p); return true; }
	case E_GameObjectEvent::TRIGGER_ENTER: { Collider2D* p = reinterpret_cast<Collider2D*>(arg); this->onTriggerEnter(*p); return true; }
	case E_GameObjectEvent::TRIGGER_STAY: { Collider2D* p = reinterpret_cast<Collider2D*>(arg); this->onTriggerStay(*p); return true; }
	case E_GameObjectEvent::TRIGGER_EXIT: { Collider2D* p = reinterpret_cast<Collider2D*>(arg); this->onTriggerExit(*p); return true; }
	case E_GameObjectEvent::ADD_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); return this->onAddComponent(comp); }
	case E_GameObjectEvent::REMOVE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onRemoveComponent(comp); return true; }
	case E_GameObjectEvent::ENABLE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onEnableComponent(comp); return true; }
	case E_GameObjectEvent::DISABLE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onDisableComponent(comp); return true; }
	case E_GameObjectEvent::APPLY_RESERVATION: this->onApplyReservation(); return true;
	case E_GameObjectEvent::MOVE_OBJECT: this->onMove(); return true;
	}
	return true;
}
#pragma endregion