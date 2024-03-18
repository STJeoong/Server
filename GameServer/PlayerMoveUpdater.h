#pragma once
#include <queue>
#include <vector>
#include "game_protocol.pb.h"
using namespace protocol::game;
class PlayerMoveUpdater
{
public:
	PlayerMoveUpdater();
	~PlayerMoveUpdater();
private:
	void onMoveReq(int serial, Move_Req& req);
	void update();

	std::queue<std::pair<int, Move_Req>> _queue;
};

