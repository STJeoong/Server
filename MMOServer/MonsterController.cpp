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
	Map* map = _me->map();
	auto path = map->findPath(_me->transform(), other.gameObject()->transform());
	if (!path.has_value()) return; // 타겟으로 이동할 수 없으면 타겟 x
	_target = reinterpret_cast<Player*>(other.gameObject());
	this->move(path.value());
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
			this->move((E_Dir)dir);
			break;
		}
	}
}
void MonsterController::decisionWhenTargetExist()
{
	Map* map = _me->map();
	auto path = map->findPath(_me->transform(), _target->transform());
	if (!path.has_value())
	{
		_target = this->findNewTarget();
		return;
	}
	this->move(path.value());
}
Player* MonsterController::findNewTarget()
{
	Map* map = _me->map();
	for (Area* area : _aggressiveArea->overlappedAreas())
	{
		auto path = map->findPath(_me->transform(), area->gameObject()->transform());
		if (path.has_value())
		{
			this->move(path.value());
			return reinterpret_cast<Player*>(area->gameObject());
		}
	}
	return nullptr;
}
void MonsterController::move(protocol::mmo::E_Dir dir)
{
	_me->state(E_ObjectState::MOVE);
	int ny = _me->transform().y() + dy[(int)dir];
	int nx = _me->transform().x() + dx[(int)dir];
	int speed = _me->stats().defaultSpeed; // TODO : speed값으로
	_me->transform(ny, nx, dir);
	_nextUpdateCnt = (1600 / speed) / Game::UPDATE_DELTA_TIME;

	Move_Notify notify = {};
	notify.set_dir(dir);
	notify.set_id(_me->id());
	_me->broadcast(E_PacketID::MOVE_NOTIFY, notify);
}
#pragma endregion
