#include <windows.h>
#include "UserManager.h"
#include "User.h"
#include "Engine.h"
#pragma region public
User* UserManager::getUser(int serial) const { return &(_users[serial]); }
#pragma endregion

#pragma region private
UserManager::UserManager()
{
	_users = new User[Engine::getInstance().getMaxClient()];
}
#pragma endregion