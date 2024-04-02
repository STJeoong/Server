#pragma once
#include "S_EngineEvent.h"
class I_EngineEventContainer
{
public:
	virtual ~I_EngineEventContainer() = default;
	virtual void enqueue(int engineID, S_EngineEvent evt) = 0;
	virtual std::pair<int, S_EngineEvent> pop() = 0;
};

