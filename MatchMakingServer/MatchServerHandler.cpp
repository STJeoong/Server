#include "MatchServerHandler.h"
#include <S_PacketHeader.h>

#pragma region public
void MatchServerHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", evt.serial); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", evt.serial); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
}
#pragma endregion

#pragma region private
#pragma endregion