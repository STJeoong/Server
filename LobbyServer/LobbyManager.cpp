#include "LobbyManager.h"
#include "S_UserInfo.h"
#include "Engine.h"
#include "MemoryBlockPoolManager.h"
#include "lobby_protocol.pb.h"
#include "Server.h"
#include <S_PacketHeader.h>

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
void LobbyManager::broadcast(int serial, char* data)
{
	S_UserInfo& info = *(this->getUser(serial));
	S_PacketAttr attr = {};
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(data);
	protocol::lobby::ChatLobby_Req req = {};
	protocol::lobby::ChatLobby_Notify notify = {};

	attr.packetID = (UINT16)protocol::lobby::E_PacketID::CHAT_LOBBY_NOTIFY;
	attr.option = 0;
	req.ParseFromArray(data + sizeof(S_PacketHeader), header->initLen);
	std::string name = info.name;
	std::string con = req.content();
	printf("chat req : (%s), content : (%s)\n", info.name.c_str(), req.content().c_str());
	notify.set_name(info.name);
	notify.set_content(req.content());

	for (int i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->serial == serial)
			continue;
		Server::getInstance().send(E_EngineType::LOBBY_SERVER, _users[i]->serial, attr, notify);
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