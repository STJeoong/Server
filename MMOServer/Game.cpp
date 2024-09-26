#include "Game.h"
#include <iostream>
#include <stack>
#include "MMOServerBroadcaster.h"
#include <Engine.h>
#include "E_EngineType.h"
#include "E_TimerEvent.h"
#include "Map.h"
#include "Monster.h"
#include "GameObject.h"
#include "Component.h"
#include "Area.h"
#include "LayerFilter.h"
#include "Player.h"
#include "Skill.h"
#include "ActionTimer.h"
#include "Buff.h"
#include "CC.h"	
#include "PersistentHit.h"
#include <ObjectPool.h>

#pragma region public static
void Game::init()
{
	MMOServerBroadcaster::onConnect += [](int serial) { printf("%d connect\n", serial); };
	MMOServerBroadcaster::onDisconnect += [](int serial) { printf("%d disconnect\n", serial); };
	MMOServerBroadcaster::onUpdate += []()
	{
		s_updateCnt += Game::UPDATE_DELTA_TIME;
		Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::UPDATE_DELTA_TIME, (int)E_TimerEvent::UPDATE);
	};
	MMOServerBroadcaster::onMonsterRespawn += []()
	{
		Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::MONSTER_RESPAWN_DELTA_TIME, (int)E_TimerEvent::MONSTER_RESPAWN);
	};

	// Pooling
	ObjectPool::makePool<ActionTimer>(10000, []() { return new ActionTimer(); }, [](void* p) {}, [](void* p) {});
	ObjectPool::makePool<Buff>(10000, []() { return new Buff(); }, [](void* p) {}, [](void* p) {});
	ObjectPool::makePool<CC>(10000, []() { return new CC(); }, [](void* p) {}, [](void* p) {});
	ObjectPool::makePool<PersistentHit>(10000, []() { return new PersistentHit(); }, [](void* p) {}, [](void* p) {});
	ObjectPool::useActionOnGet<ActionTimer>(false);
	ObjectPool::useActionOnRelease<ActionTimer>(false);
	ObjectPool::useActionOnGet<Buff>(false);
	ObjectPool::useActionOnRelease<Buff>(false);
	ObjectPool::useActionOnGet<CC>(false);
	ObjectPool::useActionOnRelease<CC>(false);
	ObjectPool::useActionOnGet<PersistentHit>(false);
	ObjectPool::useActionOnRelease<PersistentHit>(false);

	Map::load();
	LayerFilter::init();
	Skill::init();
	Player::init();
	Monster::init();
	std::cout << "Server Start\n";

	// game start
	Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::UPDATE_DELTA_TIME, (int)E_TimerEvent::UPDATE);
	Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::MONSTER_RESPAWN_DELTA_TIME, (int)E_TimerEvent::MONSTER_RESPAWN);
}
#pragma endregion

long long Game::s_updateCnt = 0;