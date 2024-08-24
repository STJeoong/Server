#pragma once
#include "S_Stat.h"
#include "MMO_struct.pb.h"

using namespace protocol::mmo;
class Map;
class I_Skill
{
public:
	virtual void use(S_Stat& userStat, Map* map, TransformInt& pivot) = 0;
};