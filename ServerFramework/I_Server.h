#pragma once
#include <basetsd.h>
#include "E_EngineEvent.h"
class I_Server
{
public:
	virtual ~I_Server() {}
	virtual bool init() = 0;
	virtual void process(int serial, E_EngineEvent type, char* data) = 0;
};