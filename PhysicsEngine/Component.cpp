#include "pch.h"
#include "Component.h"
#include "GameObject.h"

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
	case E_GameObjectEvent::UPDATE: this->onUpdate(); break;
	case E_GameObjectEvent::DESTROY: this->onDestroy(); break;
	case E_GameObjectEvent::COLLISION_ENTER: this->onCollisionEnter(); break;
	case E_GameObjectEvent::COLLISION_STAY: this->onCollisionStay(); break;
	case E_GameObjectEvent::COLLISION_EXIT: this->onCollisionExit(); break;
	case E_GameObjectEvent::TRIGGER_ENTER: this->onTriggerEnter(); break;
	case E_GameObjectEvent::TRIGGER_STAY: this->onTriggerStay(); break;
	case E_GameObjectEvent::TRIGGER_EXIT: this->onTriggerExit(); break;
	case E_GameObjectEvent::ADD_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onAddComponent(comp); break; }
	case E_GameObjectEvent::REMOVE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onRemoveComponent(comp); break; }
	case E_GameObjectEvent::ENABLE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onEnableComponent(comp); break; }
	case E_GameObjectEvent::DISABLE_COMPONENT: { Component* comp = reinterpret_cast<Component*>(arg); this->onDisableComponent(comp); break; }
	}
}
void Component::onUpdate()
{
	if (!_gameObject->isActive())
		return;
	if (!_isStart)
	{
		_isStart = true;
		this->start();
	}
	else
		this->update();
}
#pragma endregion