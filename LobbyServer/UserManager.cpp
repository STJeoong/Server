#include <windows.h>
#include "UserManager.h"
#include "S_UserInfo.h"
#include "Server.h"
#pragma region public
S_UserInfo& UserManager::getUser(int serial) const { return _users[serial]; }
#pragma endregion

#pragma region private
UserManager::UserManager()
{
	_users = new S_UserInfo[Server::getInstance().getMaxClient()];
	for (int i = 0; i < Server::getInstance().getMaxClient(); ++i)
	{
		_users[i].name = "";
		_users[i].serial = i;
		_users[i].state = E_UserState::DISCONNECTED;
	}
}
#pragma endregion