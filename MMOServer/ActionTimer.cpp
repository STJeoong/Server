#include "ActionTimer.h"
#include "GameObject.h"
#include "Game.h"

#pragma region public
#pragma endregion

#pragma region private
void ActionTimer::invoke(const E_GameObjectEvent& evt, void* arg)
{
	switch (evt)
	{
	case E_GameObjectEvent::UPDATE: this->update(); break;
	case E_GameObjectEvent::INACTIVE:
	case E_GameObjectEvent::DESTROY: _gameObject->removeTimer(this); break;
	}
}
void ActionTimer::update()
{
	_delayTime -= Game::UPDATE_DELTA_TIME;
	if (_delayTime > 0)
		return;
	_action(_gameObject);
	_gameObject->removeTimer(this);
}
#pragma endregion
