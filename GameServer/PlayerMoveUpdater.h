#pragma once
#include <vector>
#include "game_protocol.pb.h"
using namespace protocol::game;
class PlayerMoveUpdater
{
public:
	PlayerMoveUpdater();
	~PlayerMoveUpdater();
private:
	void onMoveReq(int serial, const Move_Req& req);
	void update();
};

