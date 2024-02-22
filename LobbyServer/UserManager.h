#pragma once
#include "S_UserInfo.h"
#include <Singleton.h>
class User;
class UserManager : public Singleton<UserManager>
{
	friend class Singleton;
public:
	S_UserInfo& getUser(int serial) const;
private:
	UserManager();
	~UserManager() = default;

	S_UserInfo* _users;
};

