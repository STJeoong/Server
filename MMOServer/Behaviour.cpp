#include "Behaviour.h"
#include "GameObject.h"

#pragma region public
void Behaviour::enable(bool flag)
{
	if (_enabled == flag)
		return;
	_enabled = flag;
	if (!_gameObject->activeInHierarchy())
		return;

	if (_enabled) this->onEnable();
	else this->onDisable();
}
#pragma endregion

#pragma region protected
void Behaviour::onUpdate()
{
	if (!_enabled)
		return;

	if (_isFirstUpdate)	this->start();
	else this->update();
	_isFirstUpdate = false;
}
void Behaviour::onActiveGameObject()
{
	if (!_enabled)
		return;
	this->onEnable();
}
void Behaviour::onInactiveGameObject()
{
	if (!_enabled)
		return;
	this->onDisable();
}
#pragma endregion

#pragma region private
#pragma endregion