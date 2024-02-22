#pragma once
#include <string>
#include "E_UserState.h"
struct S_UserInfo
{
	int serial;
	std::string name;
	E_UserState state;
};