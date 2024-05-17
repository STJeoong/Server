#include "pch.h"
#include "World.h"
#include "Collider2D.h"
#include "GameObject.h"
#include "RigidBody2D.h"
#include "Solver.h"
#include "ContactFilter.h"
#include "Contact2D.h"
#include <ObjectPool.h>

#include <SFML/Graphics.hpp>
extern sf::RenderWindow window;
#pragma region public
GameObject* World::find(const std::string& name)
{
	for (GameObject*& obj : s_gameObjects)
		if (obj->_name == name)
			return obj;
	return nullptr;
}
GameObject* World::instantiate(GameObject* copy, GameObject* parent, bool active)
{
	if (s_root == nullptr) s_root = new GameObject();
	GameObject* ret = new GameObject();
	ret->isActive(active);
	if (copy != nullptr) *ret = *copy;
	if (parent != nullptr) ret->setParent(*parent);
	else ret->setParent(*s_root);
	s_gameObjects.push_back(ret);
	return ret;
}
void World::destroy(GameObject*& obj)
{
	auto it = std::find(s_gameObjects.begin(), s_gameObjects.end(), obj);
	s_gameObjects.erase(it);
	s_destroyed.push_back(obj);
	obj = nullptr;
}

void World::init(const Vector2D& g)
{
	s_gravity = g;
	ContactFilter::reset();
	ObjectPool::makePool<Collision2D>(World::COLLISION_POOL_SIZE, []{ return new Collision2D(); },
		[](void* collision) {}, [](void* collision) { Collision2D* p = reinterpret_cast<Collision2D*>(collision); p->onDestroy(); });
	ObjectPool::makePool<Contact2D>(World::CONTACT_POOL_SIZE, [] { return new Contact2D(); },
		[](void* contact) {}, [](void* contact) { Contact2D* c = reinterpret_cast<Contact2D*>(contact); c->onDestroy(); });
}
void World::step(float dt, int velocityIter, int positionIter)
{
	for (int i = 0; i < s_gameObjects.size(); ++i)
		if (s_gameObjects[i]->isActive())
			s_gameObjects[i]->broadcast(E_GameObjectEvent::UPDATE, nullptr);

	s_broadPhase.update();
	s_detector.update(s_broadPhase);

	s_solver.init(s_detector.collisions());
	s_solver.integrateVelocity(s_rigids, s_gravity, dt);
	for (int i = 0; i < velocityIter; ++i)
		s_solver.solveVelocityConstraints(s_detector.collisions());
	s_solver.integratePosition(s_rigids, dt);
	for (int i = 0; i < positionIter; ++i)
		s_solver.solvePositionConstraints(s_detector.collisions());

	for (RigidBody2D* rigid : s_rigids)
		rigid->sync();
	// TODO : continuous collision
	// update broad-phase

	World::invokeCollisionEvents();
	World::invokeExitEvents();
	World::destroyObjects();
	World::removeCollisions();
	World::removeColliders();

	for (int i = 0; i < s_gameObjects.size(); ++i)
		if (s_gameObjects[i]->isActive())
			s_gameObjects[i]->applyReservation();
}
#pragma endregion

#pragma region private
int World::add(Collider2D* collider) { return s_broadPhase.insert(collider, collider->computeAABB()); }
int World::add(RigidBody2D* rigid) { s_rigids.push_back(rigid); return (int)s_rigids.size() - 1; }
void World::moveCollider(int key, const AABB& sweepAABB, const Vector2D& displacement) { s_broadPhase.move(key, sweepAABB, displacement); }
void World::removeRigid(int key) { s_rigids.erase(s_rigids.begin() + key); }
void World::removalReq(int key)
{
	//s_broadPhase.remove(key); TODO : 바로 삭제해도 되나? 나중에 시뮬레이션할때 문제 생기나? 아니면 바로 지워도 될거같은데
	s_removals.push(key);
	Collider2D* collider = reinterpret_cast<Collider2D*>(s_broadPhase.getData(key));
	s_detector.findRelatedCollisions(collider, s_exits);
}
void World::invokeCollisionEvents()
{
	const std::vector<Collision2D*> collisions = s_detector.collisions();
	for (Collision2D* c : collisions)
	{
		Collider2D* colliderA = c->colliderA();
		Collider2D* colliderB = c->colliderB();
		GameObject* objA = colliderA->gameObject();
		GameObject* objB = colliderB->gameObject();
		const E_GameObjectEvent& evt = c->_evt;

		if (evt == E_GameObjectEvent::COLLISION_ENTER || evt == E_GameObjectEvent::COLLISION_STAY)
		{
			objA->broadcast(evt, c);
			objB->broadcast(evt, c);
		}
		else if (evt == E_GameObjectEvent::TRIGGER_ENTER || evt == E_GameObjectEvent::TRIGGER_STAY)
		{
			objA->broadcast(evt, colliderB);
			objB->broadcast(evt, colliderA);
		}
	}
}
void World::invokeExitEvents()
{
	size_t idx = 0;
	Collider2D* colliderA = nullptr;
	Collider2D* colliderB = nullptr;
	GameObject* objA = nullptr;
	GameObject* objB = nullptr;

	while (true)
	{
		size_t before = s_exits.size();
		for (; idx < before; ++idx)
		{
			const E_GameObjectEvent& evt = s_exits[idx]->_evt;
			colliderA = s_exits[idx]->colliderA();
			colliderB = s_exits[idx]->colliderB();
			objA = colliderA->gameObject();
			objB = colliderB->gameObject();

			if (evt == E_GameObjectEvent::COLLISION_ENTER || evt == E_GameObjectEvent::COLLISION_STAY)
			{
				objA->broadcast(E_GameObjectEvent::COLLISION_EXIT, s_exits[idx]);
				objB->broadcast(E_GameObjectEvent::COLLISION_EXIT, s_exits[idx]);
			}
			else if (evt == E_GameObjectEvent::TRIGGER_ENTER || evt == E_GameObjectEvent::TRIGGER_STAY)
			{
				objA->broadcast(E_GameObjectEvent::TRIGGER_EXIT, colliderB);
				objB->broadcast(E_GameObjectEvent::TRIGGER_EXIT, colliderA);
			}
		}
		size_t after = s_exits.size();
		if (before == after) return;
	}
}
void World::destroyObjects()
{
	auto it = s_destroyed.begin();
	while (it != s_destroyed.end())
	{
		(*it)->broadcast(E_GameObjectEvent::DESTROY, nullptr);
		delete (*it);
		it = s_destroyed.erase(it);
	}
}
void World::removeColliders()
{
	while (!s_removals.empty())
	{
		int key = s_removals.front();
		s_removals.pop();
		s_broadPhase.remove(key);
	}
}
void World::removeCollisions()
{
	auto it = s_exits.begin();
	while (it != s_exits.end())
		it = World::removeCollsion(it);
}
std::vector<Collision2D*>::iterator World::removeCollsion(std::vector<Collision2D*>::iterator& it)
{
	Collision2D* collision = (*it);
	std::vector<Collision2D*>& collisionsA = collision->colliderA()->_collisions;
	std::vector<Collision2D*>& collisionsB = collision->colliderB()->_collisions;

	collisionsA.erase(std::remove(collisionsA.begin(), collisionsA.end(), collision), collisionsA.end());
	collisionsB.erase(std::remove(collisionsB.begin(), collisionsB.end(), collision), collisionsB.end());
	ObjectPool::release(collision);
	return s_exits.erase(it);
}
#pragma endregion

std::vector<GameObject*> World::s_gameObjects;
std::vector<GameObject*> World::s_destroyed;
GameObject* World::s_root;

Vector2D World::s_gravity;
BroadPhase World::s_broadPhase;
std::vector<RigidBody2D*> World::s_rigids;
CollisionDetector World::s_detector;
Solver World::s_solver;
std::queue<int> World::s_removals;
std::vector<Collision2D*> World::s_exits;