#pragma once
#include <functional>
#include "E_GameObjectEvent.h"

class GameObject;
class ActionTimer
{
	friend class GameObject;
private:
	void invoke(const E_GameObjectEvent& evt, void* arg);
	void update();
	
	GameObject* _gameObject = nullptr;
	int _delayTime = 0;
	std::function<void(GameObject*)> _action = {};
};