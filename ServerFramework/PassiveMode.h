#pragma once
#include "ServerMode.h"
class PassiveMode : public ServerMode
{
public:
	void run() override;
};

