#include "pch.h"
#include "CollisionDetector.h"
#include "BroadPhase.h"
#include "Collider2D.h"
#include "ContactFilter.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "Simplex.h"
#include <ObjectPool.h>

#pragma region public
void CollisionDetector::remove(Collider2D* collider)
{
	auto it = _collisions.begin();
	while (it < _collisions.end())
	{
		it = std::find_if(it, _collisions.end(), [&collider](Collision2D* c) { return c->colliderA() == collider || c->colliderB() == collider; });
		if (it != _collisions.end())
		{
			this->remove(*it);
			it = _collisions.erase(it);
		}
	}
}
void CollisionDetector::remove(Collision2D* collision)
{
	std::vector<Collision2D*>& collisionsA = collision->colliderA()->_collisions;
	std::vector<Collision2D*>& collisionsB = collision->colliderB()->_collisions;
	GameObject* objA = collision->colliderA()->gameObject();
	GameObject* objB = collision->colliderB()->gameObject();
	Collider2D* colliderA = collision->colliderA();
	Collider2D* colliderB = collision->colliderB();

	collisionsA.erase(std::remove(collisionsA.begin(), collisionsA.end(), collision), collisionsA.end());
	collisionsB.erase(std::remove(collisionsB.begin(), collisionsB.end(), collision), collisionsB.end());
	if (collision->_evt == E_GameObjectEvent::COLLISION_ENTER || collision->_evt == E_GameObjectEvent::COLLISION_STAY)
	{
		objA->broadcast(E_GameObjectEvent::COLLISION_EXIT, collision);
		objB->broadcast(E_GameObjectEvent::COLLISION_EXIT, collision);
	}
	else if (collision->_evt == E_GameObjectEvent::TRIGGER_ENTER || collision->_evt == E_GameObjectEvent::TRIGGER_STAY)
	{
		objA->broadcast(E_GameObjectEvent::TRIGGER_EXIT, colliderB);
		objB->broadcast(E_GameObjectEvent::TRIGGER_EXIT, colliderA);
	}
	ObjectPool::release(collision);
}
void CollisionDetector::update(const BroadPhase& broadPhase)
{
	this->removeOldCollisions(broadPhase);
	this->importFromBroadPhase(broadPhase);
	for (int i = 0; i < _collisions.size(); ++i)
		this->update(_collisions[i]);
}
void CollisionDetector::removeOldCollisions(const BroadPhase& broadPhase)
{
	for (int i = 0; i < _collisions.size(); ++i)
	{
		Collider2D* colliderA = _collisions[i]->colliderA();
		Collider2D* colliderB = _collisions[i]->colliderB();
		RigidBody2D* rigidA = _collisions[i]->rigidA();
		RigidBody2D* rigidB = _collisions[i]->rigidB();

		if (!ContactFilter::shouldCollide(*colliderA, *colliderB))
			this->remove(_collisions[i]);
		if ((rigidA == nullptr || rigidA->type() == E_BodyType::STATIC) && (rigidB == nullptr || rigidB->type() == E_BodyType::STATIC))
			this->remove(_collisions[i]);
		if ((!colliderA->isTrigger() && !colliderB->isTrigger()) &&
			(rigidA == nullptr || rigidA->type() != E_BodyType::DYNAMIC) &&
			(rigidB == nullptr || rigidB->type() != E_BodyType::DYNAMIC))
			this->remove(_collisions[i]);
		//bool activeA = rigidA != nullptr && rigidA->isAwake() && rigidA->type() != E_BodyType::STATIC;
		//bool activeB = rigidB != nullptr && rigidB->isAwake() && rigidB->type() != E_BodyType::STATIC;
		//if (!activeA && !activeB) continue; // TODO : 왜 삭제안하지??
		if (!broadPhase.getAABB(colliderA->key()).overlaps(broadPhase.getAABB(colliderB->key())))
			this->remove(_collisions[i]);
	}
}
void CollisionDetector::importFromBroadPhase(const BroadPhase& broadPhase)
{
	const std::vector<std::pair<int, int>>& pairs = broadPhase.candidates();
	for (int i = 0; i < pairs.size(); ++i)
	{
		Collider2D* colliderA = reinterpret_cast<Collider2D*>(broadPhase.getData(pairs[i].first));
		Collider2D* colliderB = reinterpret_cast<Collider2D*>(broadPhase.getData(pairs[i].second));
		RigidBody2D* rigidA = colliderA->attachedRigidBody();
		RigidBody2D* rigidB = colliderB->attachedRigidBody();

		auto it = std::find_if(colliderA->collisions().begin(), colliderA->collisions().end(),
			[&colliderB](Collision2D* c) { return c->colliderA() == colliderB || c->colliderB() == colliderB; });
		if (rigidA == rigidB) continue;
		if (!ContactFilter::shouldCollide(*colliderA, *colliderB)) continue;
		if ((rigidA == nullptr || rigidA->type() == E_BodyType::STATIC) && (rigidB == nullptr || rigidB->type() == E_BodyType::STATIC)) continue;
		if ((!colliderA->isTrigger() && !colliderB->isTrigger()) &&
			(rigidA == nullptr || rigidA->type() != E_BodyType::DYNAMIC) &&
			(rigidB == nullptr || rigidB->type() != E_BodyType::DYNAMIC)) continue;
		if (it != colliderA->collisions().end()) continue; // already exist

		// new Collision
		Collision2D* collision = ObjectPool::get<Collision2D>();
		collision->_colliderA = colliderA;
		collision->_colliderB = colliderB;
		collision->_rigidA = rigidA;
		collision->_rigidB = rigidB;
		_collisions.push_back(collision);
	}
}
void CollisionDetector::update(Collision2D* collision)
{
	Collider2D* colliderA = collision->colliderA();
	Collider2D* colliderB = collision->colliderB();
	if (!gjk(colliderA, colliderB))
		this->remove(collision);
	else
	{

	}
}
#pragma endregion

#pragma region private
bool CollisionDetector::gjk(Collider2D* colliderA, Collider2D* colliderB)
{
	Point2D pointA = colliderA->computeSupportPoint({ 1.0f,0.0f });
	Point2D pointB = colliderB->computeSupportPoint({ -1.0f,0.0f });
	Simplex simplex(pointA - pointB);

	while (true)
	{
		pointA = colliderA->computeSupportPoint(simplex.supportVec());
		pointB = colliderB->computeSupportPoint(simplex.supportVec() * -1);
		if (!simplex.insert(pointA - pointB)) return false;
		if (simplex.containsOrigin()) return true;
	}
}
#pragma endregion
