#pragma once
#include <string>
#include "E_UserState.h"
struct S_UserInfo
{
	int serial;
	int roomNum;
	std::string name;
	E_UserState state;
};