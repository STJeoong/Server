#include "LobbyManager.h"
#include "S_UserInfo.h"
#include "Engine.h"
#include "lobby_protocol.pb.h"
#include <Engine.h>
#include <S_PacketHeader.h>
#include <S_PacketAttr.h>
#include "E_EngineType.h"
#include "Serializer.h"

#pragma region public
bool LobbyManager::inputUser(S_UserInfo* user)
{
	for (int i = 0; i < _users.size(); ++i)
		if (_users[i]->name == user->name)
			return false;

	_users.push_back(user);
	return true;
}
void LobbyManager::deleteUser(S_UserInfo* user)
{
	for (int i = 0; i < _users.size(); ++i)
		if (_users[i]->name == user->name)
		{
			_users.erase(_users.begin() + i);
			return;
		}
}
void LobbyManager::broadcast(int serial, const protocol::lobby::ChatLobby_Req& req)
{
	S_UserInfo& info = *(this->getUser(serial));
	protocol::lobby::ChatLobby_Notify notify = {};

	std::string name = info.name;
	std::string con = req.content();
	printf("chat req : (%s), content : (%s)\n", info.name.c_str(), req.content().c_str());
	notify.set_name(info.name);
	notify.set_content(req.content());
	std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)protocol::lobby::E_PacketID::CHAT_LOBBY_NOTIFY, 0 }, notify);

	for (int i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->serial == serial)
			continue;
		Engine::send((int)E_EngineType::LOBBY_SERVER, _users[i]->serial, ret.first, ret.second);
	}
}
#pragma endregion

#pragma region private
S_UserInfo* LobbyManager::getUser(int serial)
{
	for (int i = 0; i < _users.size(); ++i)
		if (_users[i]->serial == serial)
			return _users[i];

	return nullptr;
}
#pragma endregion