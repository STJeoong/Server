#include "MonsterController.h"
#include "GameObject.h"
#include "Area.h"
#include "MMO_protocol.pb.h"
#include "Player.h"
#include "Map.h"
#include "Utils.h"
#include "Monster.h"
#include "Game.h"

using namespace protocol::mmo;
#pragma region public
#pragma endregion

#pragma region protected
void MonsterController::update()
{
	if (_nextUpdateCnt > 0) --_nextUpdateCnt;
	if (_nextUpdateCnt > 0) return;

	switch (_me->state())
	{
	case E_ObjectState::IDLE: this->idle(); break;
	case E_ObjectState::MOVE: this->move(); break;
	}
}
void MonsterController::onAreaEnter(Area& my, Area& other)
{
	if (_target != nullptr || my.layer() != E_Layer::MONSTER_AGGRESSIVE) return;
	// 타겟 설정
	_target = reinterpret_cast<Player*>(other.gameObject());
	_me->state(E_ObjectState::MOVE);
	_nextUpdateCnt = 0;
}
void MonsterController::onAreaExit(Area& my, Area& other)
{
	if (my.layer() != E_Layer::MONSTER_AGGRESSIVE || other.gameObject() != _target) return;
	// 새로운 타겟 찾기
	const std::vector<Area*>& areas = my.overlappedAreas();
	if (areas.empty())
	{
		_me->state(E_ObjectState::IDLE);
		_target = nullptr;
		return;
	}
	_target = reinterpret_cast<Player*>(areas[0]->gameObject());
}
#pragma endregion

#pragma region private
void MonsterController::idle()
{
	this->decisionWhenNoTarget();
}
void MonsterController::move()
{
	if (_target == nullptr)
		this->decisionWhenNoTarget();
	else
		this->decisionWhenTargetExist();
}
void MonsterController::decisionWhenNoTarget()
{
	static int dy[4] = { 1,0,-1,0 };
	static int dx[4] = { 0,-1,0,1 };
	// TODO : Data로 관리하기
	// 20% 확률로 가만히 있고 80% 확률로 주변 왔다갔다
	Map* map = _me->map();
	const TransformInt& worldTF = _me->transform();
	if (Utils::gacha(20))
	{
		_nextUpdateCnt = Utils::randomVal(10, 30); // 1~3초동안 가만히 있기
		if (_me->state() != E_ObjectState::IDLE)
		{
			Idle_Notify notify = {};
			notify.set_id(_me->id());
			_me->broadcast(E_PacketID::IDLE_NOTIFY, notify);
		}
		_me->state(E_ObjectState::IDLE);
		return;
	}
	while (true)
	{
		int dir = Utils::randomVal(0, 3);
		int ny = worldTF.y() + dy[dir];
		int nx = worldTF.x() + dx[dir];
		if (map->canGo(ny, nx))
		{
			_me->transform(ny, nx, (E_Dir)dir);
			_me->state(E_ObjectState::MOVE);
			int speed = _me->stats().defaultSpeed;
			_nextUpdateCnt = (1600 / speed) / Game::UPDATE_DELTA_TIME;

			Move_Notify notify = {};
			notify.set_dir((E_Dir)dir);
			notify.set_id(_me->id());
			_me->broadcast(E_PacketID::MOVE_NOTIFY, notify);
			break;
		}
	}
}
void MonsterController::decisionWhenTargetExist()
{
}
#pragma endregion
