#include "PlayerController.h"
#include "Area.h"
#include "Utils.h"
#include "Map.h"
#include "Player.h"
#include "Skill.h"

using namespace protocol::mmo;
#pragma region  public
void PlayerController::move(const protocol::mmo::Move_Req& req)
{
	if (req.dir() < E_Dir::RIGHT || req.dir() > E_Dir::BOTTOM)
		return; // 위조된 패킷
	const TransformInt& worldTF = _me->transform();

	int ny = worldTF.y() + dy[(int)req.dir()];
	int nx = worldTF.x() + dx[(int)req.dir()];
	if (!_me->map()->canGo(ny, nx))
		return;
	_me->transform(ny, nx, req.dir());
	_me->state(E_ObjectState::MOVE);
	if (req.dir() == E_Dir::LEFT)
		_me->flipX(true);
	else if (req.dir() == E_Dir::RIGHT)
		_me->flipX(false);

	Move_Notify notify = {};
	notify.set_id(_me->id());
	notify.set_dir(req.dir());
	_me->broadcastPacket(E_PacketID::MOVE_NOTIFY, notify);
}
void PlayerController::normalAttack()
{
	if (_normalAtk == nullptr) _normalAtk = Skill::get("PLAYER_NORMAL_ATTACK");
	_normalAtk->use(_me);
}
#pragma endregion

#pragma region protected
void PlayerController::onAreaEnter(Area& my, Area& other)
{
	if (my.layer() != E_Layer::AOI) return;

	// spawn
	ObjectEnter_Notify notify = {};
	*(notify.mutable_otherinfo()) = other.gameObject()->info();
	Utils::send(_me->networkSerial(), E_PacketID::OBJECT_ENTER_NOTIFY, 0, notify);
}
void PlayerController::onAreaExit(Area& my, Area& other)
{
	// 내가 사라진거거나 AOI가 아니면 return
	if (!_me->activeInHierarchy() || my.layer() != E_Layer::AOI) return;

	// despawn
	ObjectExit_Notify notify = {};
	notify.set_otherid(other.gameObject()->id());
	Utils::send(_me->networkSerial(), E_PacketID::OBJECT_EXIT_NOTIFY, 0, notify);
}
#pragma endregion

#pragma region private
#pragma endregion