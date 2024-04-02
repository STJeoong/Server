#include "Player.h"

const float Player::INITIAL_SPEED = 5;
const float Player::FIXED_DELTA_TIME = (float)0.02;
#pragma region public
void Player::move(const Move_Req& req)
{
	// TODO : Àå¾Ö¹°
	_lpi = req.seqid();
	Vec2* pos = _state.mutable_pos();
	float x = pos->x();
	float y = pos->y();
	switch (req.dir())
	{
	case E_Direction::UP: pos->set_y(y + _speed * Player::FIXED_DELTA_TIME); break;
	case E_Direction::DOWN: pos->set_y(y - _speed * Player::FIXED_DELTA_TIME);  break;
	case E_Direction::LEFT: pos->set_x(x - _speed * Player::FIXED_DELTA_TIME); break;
	case E_Direction::RIGHT: pos->set_x(x + _speed * Player::FIXED_DELTA_TIME); break;
	}
}
void Player::write(PlayerState* state)
{
	Vec2* pos = state->mutable_pos();

	state->set_id(_state.id());
	pos->set_x(_state.pos().x());
	pos->set_y(_state.pos().y());
}
#pragma endregion

#pragma region private

#pragma endregion

