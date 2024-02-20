#pragma once
#include "E_UserState.h"
#include "S_UserInfo.h"

class User
{
public:
	User();
	void connect() { _info.state = E_UserState::CONNECTED; }
	void disconnect() { _info.state = E_UserState::DISCONNECTED; }
	S_UserInfo& getInfo() { return _info; }
private:
	S_UserInfo _info;
};

