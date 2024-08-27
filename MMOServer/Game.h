#pragma once
class Game
{
public:
	static const int UPDATE_DELTA_TIME = 100;
	static const int MONSTER_RESPAWN_DELTA_TIME = 5000;

	static void init();
	// ���� ���� �� ���� �ð�(millisecond). Game::UPDATE_DELTA_TIME������ ����
	static long long elapsedTime() { return s_updateCnt; }
private:
	static long long s_updateCnt;
};