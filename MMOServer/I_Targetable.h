#pragma once
#include "MMO_protocol.pb.h"
#include "S_Stats.h"

class Buff;
class CC;
class PersistentChangeStats;
class I_Targetable
{
public:
	virtual ~I_Targetable() = default;
	virtual void addBuff(Buff* buff) = 0;
	virtual void removeBuff(Buff* buff) = 0;
	virtual void addCC(CC* cc) = 0;
	virtual void removeCC(CC* cc) = 0;
	virtual void addPersistentChangeStats(PersistentChangeStats* persistent) = 0;
	virtual void removePersistentChangeStats(PersistentChangeStats* persistent) = 0;
	virtual void changeStats(S_Stats delta) = 0;
};