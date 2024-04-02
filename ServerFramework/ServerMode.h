#pragma once
#include <unordered_map>

class I_Broadcaster;
class EngineEventContainer;
class ServerMode
{
	friend class Engine;
protected:
	virtual ~ServerMode() = default;

	std::unordered_map<int, I_Broadcaster*> _broadcasters;
	EngineEventContainer* _evtContainer = nullptr;
	bool _isRun = false;
private:
	virtual void run() = 0;
	void setEventContainer(EngineEventContainer* evtContainer) { _evtContainer = evtContainer; }
	bool addBroadcaster(int id, I_Broadcaster* broadcaster) { if (_broadcasters.find(id) != _broadcasters.end()) return false; _broadcasters[id] = broadcaster; return true; }
};