#pragma once
#include <Singleton.h>
#include <unordered_map>
#include <vector>

class Player;
class PlayerManager : public Singleton<PlayerManager>
{
	friend class Singleton;
public:
	int size() { return _mp.size(); }
	bool newPlayer(int serial);
	bool deletePlayer(int serial);
	Player* getPlayer(int serial) { if (_mp.find(serial) == _mp.end()) return nullptr; return _mp[serial]; }
	const std::unordered_map<int, Player*>& getAllPlayers() const { return _mp; }
private:
	PlayerManager() = default;
	~PlayerManager() = default;

	std::unordered_map<int, Player*> _mp;
};

