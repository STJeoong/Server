#pragma once
#include "S_UserInfo.h"
#include "lobby_protocol.pb.h"
#include <Singleton.h>
#include <vector>
class LobbyManager : public Singleton<LobbyManager>
{
	friend class Singleton;
public:
	bool inputUser(S_UserInfo* user);
	void deleteUser(S_UserInfo* user);
	void broadcast(int serial, const protocol::lobby::ChatLobby_Req& req);
private:
	LobbyManager() = default;
	~LobbyManager() = default;

	S_UserInfo* getUser(int serial);

	std::vector<S_UserInfo*> _users;
};

