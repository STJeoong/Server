#pragma once
#include <Singleton.h>
class User;
class UserManager : public Singleton<UserManager>
{
	friend class Singleton;
public:
	User* getUser(int serial) const;
private:
	UserManager();
	~UserManager() = default;

	User* _users;
};

