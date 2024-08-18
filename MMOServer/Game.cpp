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

#pragma region public
void Game::init()
{
	Map::load();
	LayerFilter::init();
	Monster::init();

	MMOServerBroadcaster::onConnect += [](int serial) { printf("%d connect\n", serial); };
	MMOServerBroadcaster::onDisconnect += [](int serial) { printf("%d disconnect\n", serial); };
	MMOServerBroadcaster::onUpdate += Game::update;

	Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::UPDATE_DELTA_TIME, (int)E_TimerEvent::UPDATE);
}
GameObject* Game::instantiate(GameObject* copy, bool active)
{
	return nullptr;
}
void Game::destroy(GameObject* obj)
{
	obj->broadcastToComponents(E_GameObjectEvent::DESTROY, nullptr);
	s_gameObjects.erase(std::find(s_gameObjects.begin(), s_gameObjects.end(), obj));
	s_destroyedGameObjects.push(obj);

	if (obj->_parent != nullptr)
	{
		std::vector<GameObject*>& children = obj->_parent->_children;
		children.erase(std::find(children.begin(), children.end(), obj));
	}
	if (!obj->_children.empty())
	{
		static std::stack<GameObject*> stk; // for performance, this variable is static.
		for (GameObject* child : obj->_children)
			stk.push(child);
		while (!stk.empty())
		{
			GameObject* g = stk.top();
			stk.pop();
			for (GameObject* child : g->_children)
				stk.push(child);
			g->broadcastToComponents(E_GameObjectEvent::DESTROY, nullptr);
			s_gameObjects.erase(std::find(s_gameObjects.begin(), s_gameObjects.end(), g));
			s_destroyedGameObjects.push(g);
		}
	}
}
void Game::destroy(Component* component)
{
	component->invoke(E_GameObjectEvent::DESTROY, nullptr);
	std::vector<Component*>& components = component->gameObject()->_components;
	components.erase(std::find(components.begin(), components.end(), component));
	s_destroyedComponents.push(component);
}
#pragma endregion

#pragma region private
void Game::update()
{
	Engine::setTimer((int)E_EngineType::MMO_SERVER, 0, Game::UPDATE_DELTA_TIME, (int)E_TimerEvent::UPDATE);
	for (GameObject* obj : s_gameObjects)
		if (obj->activeInHierarchy())
			obj->broadcastToComponents(E_GameObjectEvent::UPDATE, nullptr);

	// discrete collision detection. 이전에 하려고 했던 움직일때마다 체크하는건 continuous collision detection.
	// TODO : broad-phase, narrow-phase
	s_dat.makeCandidates(s_queryIds, s_candidates);
	Game::updateCollisions();
	Game::destroy();
	s_queryIds.clear();
	s_candidates.clear();
}
void Game::destroy()
{
	while (!s_destroyedGameObjects.empty())
	{
		GameObject* obj = s_destroyedGameObjects.front();
		s_destroyedGameObjects.pop();
		delete obj;
	}
	while (!s_destroyedComponents.empty())
	{
		Component* component = s_destroyedComponents.front();
		s_destroyedComponents.pop();
		delete component;
	}
}
void Game::updateCollisions()
{
	// delete invalid collisions.
	for (auto it = s_collisions.begin(); it != s_collisions.end(); ++it)
	{
		Area* areaA = it->first;
		Area* areaB = it->second;
		if (!LayerFilter::detectable(areaA->layer(), areaB->layer()) || 
			(areaA->gameObject() == areaB->gameObject() && !areaA->detectMyArea() && !areaB->detectMyArea()))
		{
			Game::invokeAreaEvent(E_GameObjectEvent::AREA_EXIT, *it);
			it = s_collisions.erase(it);
			continue;
		}

		// 후보군에 같은 게 있는지 판별
		auto candidateIt = std::find_if(s_candidates.begin(), s_candidates.end(), [&areaA, &areaB](const std::pair<int, int>& p)
			{ return (areaA == s_dat.getData(p.first) && areaB == s_dat.getData(p.second)) || (areaA == s_dat.getData(p.second) && areaB == s_dat.getData(p.first));  });
		if (candidateIt == s_candidates.end()) // 기존의 collision은 invalid => 기존 목록에서 제거
		{
			Game::invokeAreaEvent(E_GameObjectEvent::AREA_EXIT, *it);
			it = s_collisions.erase(it);
		}
		else // 기존은 collision은 여전히 valid => 후보군에서 제외
			s_candidates.erase(candidateIt);
	}

	for (int i = 0; i < s_collisions.size(); ++i)
		Game::invokeAreaEvent(E_GameObjectEvent::AREA_STAY, s_collisions[i]);

	// import valid collisions.
	for (int i = 0; i < s_candidates.size(); ++i)
	{
		Area* areaA = reinterpret_cast<Area*>(s_dat.getData(s_candidates[i].first));
		Area* areaB = reinterpret_cast<Area*>(s_dat.getData(s_candidates[i].second));
		if (!LayerFilter::detectable(areaA->layer(), areaB->layer()) ||
			(areaA->gameObject() == areaB->gameObject() && !areaA->detectMyArea() && !areaB->detectMyArea()))
			continue;
		std::pair<Area*, Area*> collision = { areaA, areaB };
		Game::invokeAreaEvent(E_GameObjectEvent::AREA_ENTER, collision);
		s_collisions.push_back(collision);
	}
}
void Game::invokeAreaEvent(const E_GameObjectEvent& evt, const std::pair<Area*, Area*>& collision)
{
	std::pair<Area*, Area*> a = collision;
	std::pair<Area*, Area*> b = { collision.second, collision.first };
	collision.first->gameObject()->broadcastToComponents(evt, &a);
	collision.second->gameObject()->broadcastToComponents(evt, &b);
}
int Game::add(void* userData, const AABB& aabb)
{
	int id = s_dat.insert(userData, aabb);
	s_queryIds.push_back(id);
	return id;
}
void Game::remove(int id)
{
	{
		auto it = std::find(s_queryIds.begin(), s_queryIds.end(), id);
		if (it != s_queryIds.end())
			s_queryIds.erase(it);
	}
	Area* area = reinterpret_cast<Area*>(s_dat.getData(id));
	s_dat.remove(id);

	// collision 중에서 id에 해당하는 area와 관련된 것들 제거
	{
		auto it = s_collisions.begin();
		while (it != s_collisions.end())
		{
			it = std::find_if(it, s_collisions.end(), [&area](const std::pair<Area*, Area*>& p) { return area == p.first || area == p.second; });
			if (it != s_collisions.end())
			{
				Game::invokeAreaEvent(E_GameObjectEvent::AREA_EXIT, *it);
				it = s_collisions.erase(it);
			}
		}
	}
}
void Game::move(int id, const AABB& newAABB)
{
	auto it = std::find(s_queryIds.begin(), s_queryIds.end(), id);
	if (it == s_queryIds.end())
		s_queryIds.push_back(id);
	s_dat.move(id, newAABB);
}
#pragma endregion

DAT Game::s_dat;
std::vector<int> Game::s_queryIds;
std::vector<std::pair<int, int>> Game::s_candidates;
std::vector<std::pair<Area*, Area*>> Game::s_collisions;

std::vector<GameObject*> Game::s_gameObjects;
std::queue<GameObject*> Game::s_destroyedGameObjects;
std::queue<Component*> Game::s_destroyedComponents;