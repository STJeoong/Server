#pragma once
#include "MMO_protocol.pb.h"

class Buff;
class CC;
class PersistentHit;
class I_Targetable
{
public:
	virtual ~I_Targetable() = default;
	virtual void addBuff(Buff* buff) = 0;
	virtual void removeBuff(Buff* buff) = 0;
	virtual void addCC(CC* cc) = 0;
	virtual void removeCC(CC* cc) = 0;
	virtual void addPersistentHit(PersistentHit* persistentHit) = 0;
	virtual void removePersistentHit(PersistentHit* persistentHit) = 0;
	virtual void takeDamage(protocol::mmo::E_Stats what, int val) = 0;
};