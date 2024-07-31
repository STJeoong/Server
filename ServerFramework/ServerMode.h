#pragma once
#include "Engine.h"

class I_Broadcaster;
class I_EngineEventContainer;
class ServerMode
{
	friend class Engine;
protected:
	virtual ~ServerMode() = default;

	I_Broadcaster* _broadcasters[Engine::MAX_ENGINE] = {};
	I_EngineEventContainer* _evtContainer = nullptr;
	bool _isRun = false;
private:
	virtual void run() = 0;
	void setEventContainer(I_EngineEventContainer* evtContainer) { _evtContainer = evtContainer; }
	void addBroadcaster(int id, I_Broadcaster* broadcaster) { _broadcasters[id] = broadcaster; }
};