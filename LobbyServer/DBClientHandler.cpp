#include "DBClientHandler.h"
#include "E_DBPacketID.h"
#include <S_PacketHeader.h>

#pragma region public
void DBClientHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: puts("DB connected"); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: puts("DB disconnected"); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_DBPacketID)header->id)
	{
	case E_DBPacketID::LOGIN_RESPONSE: break;
	}
}
#pragma endregion

#pragma region private
#pragma endregion