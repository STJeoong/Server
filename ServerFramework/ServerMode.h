#pragma once
#include <unordered_map>

class I_Broadcaster;
class I_EngineEventContainer;
class ServerMode
{
	friend class Engine;
protected:
	virtual ~ServerMode() = default;

	std::unordered_map<int, I_Broadcaster*> _broadcasters;
	I_EngineEventContainer* _evtContainer = nullptr;
	bool _isRun = false;
private:
	virtual void run() = 0;
	void setEventContainer(I_EngineEventContainer* evtContainer) { _evtContainer = evtContainer; }
	bool addBroadcaster(int id, I_Broadcaster* broadcaster) { if (_broadcasters.find(id) != _broadcasters.end()) return false; _broadcasters[id] = broadcaster; return true; }
};