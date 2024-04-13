#include "pch.h"
#include "PhysicsMode.h"
#include "S_EngineEvent.h"
#include "I_EngineEventContainer.h"
#include "I_Broadcaster.h"
#include "MemoryBlockPool.h"

#pragma region public
void PhysicsMode::run()
{
	if (_isRun)
		return;
	_isRun = true;

	int engineType;
	S_EngineEvent evt;
	while (true)
	{
		while (true)
		{
			std::tie(engineType, evt) = _evtContainer->pop();
			if (evt.type == E_EngineEventType::NONE)
				break;
			_broadcasters[engineType]->broadcast(evt);

			if (evt.type == E_EngineEventType::EVENT_NET_RECV)
				MemoryBlockPool::release((int)evt.blockSize, evt.data);
		}
		// TODO : Physics
	}
}
#pragma endregion

#pragma region private
#pragma endregion