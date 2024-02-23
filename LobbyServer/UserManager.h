#pragma once
#include "S_UserInfo.h"
#include <vector>
#include <Singleton.h>
class User;
class UserManager : public Singleton<UserManager>
{
	friend class Singleton;
public:
	S_UserInfo& getUser(int serial);
	void disconnect(int serial);
private:
	UserManager();
	~UserManager() = default;

	std::vector<S_UserInfo> _users;
};

