#include "Component.h"
#include "Area.h"
#include "GameObject.h"

using namespace protocol::mmo;
#pragma region public
#pragma endregion

#pragma region protected
void Component::invokeAll(const E_GameObjectEvent& evt, void* arg)
{
	const std::vector<Component*> components = _gameObject->components();
	for (Component* component : components)
		component->invoke(evt, arg);
}
#pragma endregion

#pragma region private
void Component::invoke(const E_GameObjectEvent& evt, void* arg)
{
	switch (evt)
	{
	case E_GameObjectEvent::UPDATE: this->onUpdate(); return;
	case E_GameObjectEvent::ACTIVE: this->onActiveGameObject(); return;
	case E_GameObjectEvent::INACTIVE: this->onInactiveGameObject(); return;
	case E_GameObjectEvent::DESTROY: this->onDestroy(); return;
	}

	std::pair<Area*, Area*>* collision = reinterpret_cast<std::pair<Area*, Area*>*>(arg);
	switch (evt)
	{
	case E_GameObjectEvent::AREA_ENTER: this->onAreaEnter(*(collision->first), *(collision->second)); break;
	case E_GameObjectEvent::AREA_STAY: this->onAreaStay(*(collision->first), *(collision->second)); break;
	case E_GameObjectEvent::AREA_EXIT: this->onAreaExit(*(collision->first), *(collision->second)); break;
	}
}
#pragma endregion