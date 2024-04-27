#pragma once
#include "Component.h"
#include "E_GameObjectEvent.h"

class Behaviour : public Component
{
public:
	const bool& enabled() const { return _enabled; }
	void enable(bool flag)
	{
		if (_enabled == flag) return;
		_enabled = flag;
		if (_enabled) this->invokeAll(E_GameObjectEvent::DISABLE_COMPONENT, this);
		else this->invokeAll(E_GameObjectEvent::ENABLE_COMPONENT, this);
	}
protected:
	virtual ~Behaviour() = default;
private:
	bool _enabled = true;
};