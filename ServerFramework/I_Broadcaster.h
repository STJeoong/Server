#pragma once
#include "S_EngineEvent.h"

class I_Broadcaster
{
public:
	virtual ~I_Broadcaster() = default;
	virtual void broadcast(const S_EngineEvent& evt) = 0;
};