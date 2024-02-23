#include <windows.h>
#include "UserManager.h"
#include "S_UserInfo.h"
#include "Server.h"
#include "match_protocol.pb.h"

using namespace protocol::match;
#pragma region public
S_UserInfo& UserManager::getUser(int serial) { return _users[serial]; }
void UserManager::disconnect(int serial)
{
	if (_users[serial].state != E_UserState::MATCHING)
	{
		_users[serial].name = "";
		_users[serial].state = E_UserState::DISCONNECTED;
		return;
	}

	MatchCancle_Req req = {};
	req.set_serial(serial);
	Server::getInstance().send(E_EngineType::MATCH_CLIENT, 0, { (int)E_PacketID::MATCH_CANCLE_REQ, 0 }, req);
}
#pragma endregion

#pragma region private
UserManager::UserManager()
{
	_users = std::vector<S_UserInfo>(Server::getInstance().getMaxClient());
	for (int i = 0; i < _users.size(); ++i)
	{
		_users[i].name = "";
		_users[i].serial = i;
		_users[i].state = E_UserState::DISCONNECTED;
	}
}
#pragma endregion