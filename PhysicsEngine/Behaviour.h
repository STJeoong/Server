#pragma once
#include "Component.h"
#include "E_GameObjectEvent.h"

class Behaviour : public Component
{
public:
	const bool& enabled() const { return _enabled; }
	void enabled(bool flag)
	{
		if (_enabled == flag) return;
		if (_needToToggleEnabled) return;
		_needToToggleEnabled = true;
		if (_enabled) this->invokeAll(E_GameObjectEvent::DISABLE_COMPONENT, this);
		else this->invokeAll(E_GameObjectEvent::ENABLE_COMPONENT, this);
	}
protected:
	Behaviour() = default;
	virtual ~Behaviour() = default;
	virtual void onUpdate() override final
	{
		if (!_enabled) return;
		if (_isFirstUpdate)
		{
			_isFirstUpdate = false;
			this->start();
		}
		else
			this->update();
	}
	virtual void start() {}
	virtual void update() {}
	virtual void onApplyReservation() override
	{
		if (_needToToggleEnabled) _enabled = !_enabled;
		_needToToggleEnabled = false;
	}
private:
	bool _enabled = true;
	bool _isFirstUpdate = true;

	// reservation
	bool _needToToggleEnabled = false;
};