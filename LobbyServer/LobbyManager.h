#pragma once
#include <Singleton.h>
#include <vector>
class User;
class LobbyManager : public Singleton<LobbyManager>
{
	friend class Singleton;
public:
	bool inputUser(User* user);
	void deleteUser(User* user);
	void broadcast(int serial, char* data);
private:
	LobbyManager() = default;
	~LobbyManager() = default;

	User* getUser(int serial);

	std::vector<User*> _users;
};

