#pragma once
#include "Component.h"
class Behaviour : public Component
{
public:
	void enable(bool flag);
	bool enable() const { return _enabled; }
protected:
	Behaviour() = default;
	virtual ~Behaviour() = default;
	virtual void onUpdate() override final;
	virtual void onActiveGameObject() override final;
	virtual void onInactiveGameObject() override final;
	virtual void onEnable() {}
	virtual void onDisable() {}
	virtual void start() {}
	virtual void update() {}

	bool _enabled = true;
private:

	bool _isFirstUpdate = true;
};