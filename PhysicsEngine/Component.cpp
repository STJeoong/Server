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
void Component::invoke(const E_GameObjectEvent& evt, void* arg)
{
	switch (evt)
	{
	case E_GameObjectEvent::ACTIVE: this->onActiveGameObject(); break;
	case E_GameObjectEvent::INACTIVE: this->onInactiveGameObject(); break;
	case E_GameObjectEvent::UPDATE: { float* dt = reinterpret_cast<float*>(arg); this->onUpdate(*dt); break; }
	case E_GameObjectEvent::DESTROY: this->onDestroy(); break;
	case E_GameObjectEvent::COLLISION_ENTER: { Collision2D* p = reinterpret_cast<Collision2D*>(arg); this->onCollisionEnter(*p); break; }
	case E_GameObjectEvent::COLLISION_STAY: { Collision2D* p = reinterpret_cast<Collision2D*>(arg); this->onCollisionStay(*p); break; }
	case E_GameObjectEvent::COLLISION_EXIT: { Collision2D* p = reinterpret_cast<Collision2D*>(arg); this->onCollisionExit(*p); break; }
	case E_GameObjectEvent::TRIGGER_ENTER: { Collider2D* p = reinterpret_cast<Collider2D*>(arg); this->onTriggerEnter(*p); break; }
	case E_GameObjectEvent::TRIGGER_STAY: { Collider2D* p = reinterpret_cast<Collider2D*>(arg); this->onTriggerStay(*p); break; }
	case E_GameObjectEvent::TRIGGER_EXIT: { Collider2D* p = reinterpret_cast<Collider2D*>(arg); this->onTriggerExit(*p); break; }
	case E_GameObjectEvent::ADD_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onAddComponent(comp); break; }
	case E_GameObjectEvent::REMOVE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onRemoveComponent(comp); break; }
	case E_GameObjectEvent::ENABLE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onEnableComponent(comp); break; }
	case E_GameObjectEvent::DISABLE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onDisableComponent(comp); break; }
	}
}
void Component::onUpdate(float dt)
{
	if (!_gameObject->isActive())
		return;
	if (!_isStart)
	{
		_isStart = true;
		this->start();
	}
	else
		this->update(dt);
}
#pragma endregion