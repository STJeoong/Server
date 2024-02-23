#include "MatchClientHandler.h"
#include <S_PacketHeader.h>

#pragma region public
void MatchClientHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: puts("match server connected"); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: puts("match server disconnected"); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
}
#pragma endregion

#pragma region private
#pragma endregion