#include "pch.h"
#include "PassiveMode.h"
#include "I_Broadcaster.h"
#include "EngineEventContainer.h"
#include "MemoryBlockPool.h"

#pragma region public
void PassiveMode::run()
{
	if (_isRun)
		return;
	_isRun = true;

	int engineType;
	S_EngineEvent evt;
	while (true)
	{
		std::tie(engineType, evt) = _evtContainer->pop();
		_broadcasters[engineType]->broadcast(evt);

		if (evt.type == E_EngineEventType::EVENT_NET_RECV)
			MemoryBlockPool::release(evt.blockSize, evt.data);
	}
}
#pragma endregion

#pragma region private
#pragma endregion