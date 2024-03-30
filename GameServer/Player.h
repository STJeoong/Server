#pragma once
#include "game_protocol.pb.h"
using namespace protocol::game;
class Player
{
	static const float INITIAL_SPEED;
	static const int INITIAL_POWER = 1;
	static const float FIXED_DELTA_TIME;
	static const int NO_UPDATE = -1;
public:
	Player(int id) { _state.set_id(id); Vec2* pos = _state.mutable_pos(); pos->set_x(0); pos->set_y(0); }
	void setPosition(float x, float y) { Vec2* pos = _state.mutable_pos(); pos->set_x(x); pos->set_y(y); }
	void move(Move_Req& req);
	void write(PlayerState* state);
	bool hasProcessed() { return _lastLpi != _lpi; }
	int lpi() { if (_lastLpi == _lpi) return Player::NO_UPDATE; _lastLpi = _lpi; return _lpi; }
private:

	PlayerState _state = {};
	float _speed = Player::INITIAL_SPEED;
	int _power = Player::INITIAL_POWER;
	int _lastLpi = Player::NO_UPDATE;
	int _lpi = Player::NO_UPDATE;
};

