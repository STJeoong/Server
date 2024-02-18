#include <S_PacketHeader.h>
#include "PacketHandler.h"
#include "E_PacketID.h"
#include "UserManager.h"
#include "User.h"
#include "LobbyManager.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <Engine.h>
#include "protocol.pb.h"

#pragma region public
void PacketHandler::handle(int serial, char* data)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::LOGIN_REQUEST: this->login(serial, data); break;
	case E_PacketID::CHAT_LOBBY_REQUEST: this->broadcastLobby(serial, data); break;
	default: LOG(ERROR) << "Undefined PacketID : " << header->id; break;
	}
}
#pragma endregion

#pragma region private
void PacketHandler::login(int serial, char* data)
{
	User* user = UserManager::getInstance().getUser(serial);
	S_UserInfo* userInfo = user->getInfo();
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(data);
	protocol::Login_Req req;

	if (userInfo->state != E_UserState::CONNECTED)
	{
		LOG(ERROR) << "Player(" << userInfo->name << ") is not connected. but request login";
		return;
	}
	req.ParseFromArray(data + sizeof(S_PacketHeader), header->initLen);
}
void PacketHandler::broadcastLobby(int serial, char* data)
{
	User* user = UserManager::getInstance().getUser(serial);
	S_UserInfo* userInfo = user->getInfo();

	if (userInfo->state != E_UserState::LOBBY && userInfo->state != E_UserState::MATCHING)
	{
		LOG(ERROR) << "Player(" << userInfo->name << ") is not in Lobby. but request broadcastLobby";
		return;
	}
	LobbyManager::getInstance().broadcast(serial, data);
}
#pragma endregion