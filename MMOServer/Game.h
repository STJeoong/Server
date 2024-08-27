#pragma once
class Game
{
public:
	static const int UPDATE_DELTA_TIME = 100;
	static const int MONSTER_RESPAWN_DELTA_TIME = 5000;

	static void init();
	// 서버 가동 후 지난 시간(millisecond). Game::UPDATE_DELTA_TIME단위로 증가
	static long long elapsedTime() { return s_updateCnt; }
private:
	static long long s_updateCnt;
};