#pragma once
#include "E_UserState.h"
#include "S_UserInfo.h"

class User
{
public:
	void connect() { _info->state = E_UserState::CONNECTED; }
	void disconnect() { _info->state = E_UserState::DISCONNECTED; }
	S_UserInfo* getInfo() const { return _info; }
private:
	S_UserInfo* _info;
};

