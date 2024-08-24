#include "PlayerController.h"
#include "Area.h"
#include "Utils.h"
#include "Map.h"

using namespace protocol::mmo;
#pragma region  public
void PlayerController::move(const protocol::mmo::Move_Req& req)
{
	static int dy[4] = { 1,0,-1,0 };
	static int dx[4] = { 0,-1,0,1 };
	const TransformInt& worldTF = _gameObject->transform();

	if (!_gameObject->map()->canGo(worldTF.y() + dy[(int)req.dir()], worldTF.x() + dx[(int)req.dir()]))
		return;
	_gameObject->transform(worldTF.y() + dy[(int)req.dir()], worldTF.x() + dx[(int)req.dir()], req.dir());
	_gameObject->state(E_ObjectState::MOVE);
	// 나를 볼 수 있는 플레이어들에게 알려주기
	Move_Notify notify = {};
	notify.set_dir(req.dir());
	notify.set_id(_gameObject->id());
	for (Area* area : _myObjArea->overlappedAreas())
		if (area->layer() == E_Layer::AOI)
			Utils::send(area->gameObject()->id(), E_PacketID::MOVE_NOTIFY, 0, notify);
	// 본인에게도 알려주기
	Utils::send(_gameObject->id(), E_PacketID::MOVE_NOTIFY, 0, notify);
}
#pragma endregion

#pragma region protected
void PlayerController::onAreaEnter(Area& my, Area& other)
{
	if (my.layer() != E_Layer::AOI) return;

	// spawn
	ObjectEnter_Notify notify = {};
	*(notify.mutable_otherinfo()) = other.gameObject()->info();
	Utils::send(_gameObject->id(), E_PacketID::OBJECT_ENTER_NOTIFY, 0, notify);
}
void PlayerController::onAreaExit(Area& my, Area& other)
{
	// 내가 사라진거거나 AOI가 아니면 return
	if (!_gameObject->activeInHierarchy() || my.layer() != E_Layer::AOI) return;

	// despawn
	ObjectExit_Notify notify = {};
	notify.set_otherid(other.gameObject()->id());
	Utils::send(_gameObject->id(), E_PacketID::OBJECT_EXIT_NOTIFY, 0, notify);
}
#pragma endregion

#pragma region private
#pragma endregion