#include "MonsterController.h"
#include "GameObject.h"
#include "Area.h"
#include "MMO_enum.pb.h"
#include "Player.h"

using namespace protocol::mmo;
#pragma region public
#pragma endregion

#pragma region protected
void MonsterController::update()
{
	if (_nextUpdateCnt > 0) --_nextUpdateCnt;
	if (_nextUpdateCnt > 0) return;

	switch (_gameObject->state())
	{
	case E_ObjectState::IDLE: break;
	case E_ObjectState::MOVE: break;
	}
}
void MonsterController::onAreaEnter(Area& my, Area& other)
{
	if (_target != nullptr || &my != _aggressiveArea) return;
	// 타겟 설정
	_target = other.gameObject()->getComponent<Player>();
}
void MonsterController::onAreaExit(Area& my, Area& other)
{
	if (&my != _aggressiveArea || other.gameObject() != _target) return;
	// 새로운 타겟 찾기
}
#pragma endregion

#pragma region private
#pragma endregion
