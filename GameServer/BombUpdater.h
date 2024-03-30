#pragma once
#include <queue>
class BombUpdater
{
public:
	BombUpdater();
	~BombUpdater();
private:
	void onBombPlantReq(int serial);
	void explosion();
	
	std::queue<std::pair<int, int>> _bombs[3];
};