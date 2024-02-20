#include "LobbyManager.h"
#include "User.h"
#include "Engine.h"
#include "MemoryBlockPoolManager.h"
#include "E_PacketID.h"
#include "protocol.pb.h"
#include "Server.h"
#include <S_PacketHeader.h>

#pragma region public
bool LobbyManager::inputUser(User* user)
{
	/*for (int i = 0; i < _users.size(); ++i)
		if (_users[i]->getName() == user->getName())
			return false;

	_users.push_back(user);*/
	return true;
}
void LobbyManager::deleteUser(User* user)
{
	/*for (int i = 0; i < _users.size(); ++i)
		if (_users[i]->getName() == user->getName())
		{
			_users.erase(_users.begin() + i);
			return;
		}*/
}
void LobbyManager::broadcast(int serial, char* data)
{
	S_UserInfo& info = this->getUser(serial)->getInfo();
	S_PacketAttr attr = {};
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(data);
	protocol::ChatLobby_Req req = {};
	protocol::ChatLobby_Notify notify = {};

	attr.packetID = (UINT16)E_PacketID::CHAT_LOBBY_NOTIFY;
	attr.option = 0;
	req.ParseFromArray(data + sizeof(S_PacketHeader), header->initLen);
	notify.set_name(info.name);
	notify.set_content(req.content());

	for (int i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->getInfo().serial == serial)
			continue;
		Server::getInstance().send(E_EngineType::LOBBY_SERVER, _users[i]->getInfo().serial, attr, notify);
	}
}
#pragma endregion

#pragma region private
User* LobbyManager::getUser(int serial)
{
	for (int i = 0; i < _users.size(); ++i)
		if (_users[i]->getInfo().serial == serial)
			return _users[i];

	return nullptr;
}
#pragma endregion