#include "PlayerManager.h"
#include "Player.h"
#pragma region public
bool PlayerManager::newPlayer(int serial)
{
	if (_mp.find(serial) != _mp.end())
		return false;
	_mp[serial] = new Player(serial);
	return true;
}
bool PlayerManager::deletePlayer(int serial)
{
	if (_mp.find(serial) == _mp.end())
		return false;
	delete _mp[serial];
	_mp[serial] = nullptr;
	_mp.erase(serial);
	return true;
}
#pragma endregion

#pragma region private

#pragma endregion