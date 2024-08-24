#pragma once
#include "MMO_struct.pb.h"
using namespace protocol::mmo;

class I_LocationBasedSkillAction
{
public:
	virtual void action(const TransformInt& location) = 0;
};