#include "pch.h"
#include "CollisionDetector.h"
#include "BroadPhase.h"
#include "Collider2D.h"
#include "ContactFilter.h"
#include "RigidBody2D.h"
#include "Simplex.h"
#include "Polytope.h"
#include "GameObject.h"
#include <ObjectPool.h>

#pragma region public
//void CollisionDetector::remove(Collider2D* collider)
//{
//	auto it = _collisions.begin();
//	while (it != _collisions.end())
//	{
//		it = std::find_if(it, _collisions.end(), [&collider](Collision2D* c) { return c->colliderA() == collider || c->colliderB() == collider; });
//		if (it != _collisions.end())
//			it = this->remove(it);
//	}
//}
void CollisionDetector::findRelatedCollisions(Collider2D* collider, std::vector<Collision2D*>& exits)
{
	auto it = _collisions.begin();
	while (it != _collisions.end())
	{
		it = std::find_if(it, _collisions.end(), [&collider](Collision2D* c) { return c->colliderA() == collider || c->colliderB() == collider; });
		if (it != _collisions.end())
		{
			exits.push_back(*it);
			it = _collisions.erase(it);
		}
	}
}
void CollisionDetector::update(const BroadPhase& broadPhase)
{
	this->removeOldCollisions(broadPhase);
	this->importFromBroadPhase(broadPhase);
	for (int i = 0; i < _collisions.size(); ++i)
		this->update(_collisions[i]);
	auto it = _collisions.begin();
	while (it != _collisions.end())
	{
		const E_GameObjectEvent& evt = (*it)->_evt;
		if (evt == E_GameObjectEvent::NONE || evt == E_GameObjectEvent::COLLISION_EXIT || evt == E_GameObjectEvent::TRIGGER_EXIT)
			it = this->remove(it);
		else
			++it;
	}
}
#pragma endregion

#pragma region private
void CollisionDetector::removeOldCollisions(const BroadPhase& broadPhase)
{
	for (int i = 0; i < _collisions.size(); ++i)
	{
		Collider2D* colliderA = _collisions[i]->colliderA();
		Collider2D* colliderB = _collisions[i]->colliderB();
		RigidBody2D* rigidA = _collisions[i]->rigidA();
		RigidBody2D* rigidB = _collisions[i]->rigidB();

		if (!ContactFilter::shouldCollide(*colliderA, *colliderB))
			this->setExit(_collisions[i]);
		if ((rigidA == nullptr || rigidA->type() == E_BodyType::STATIC) && (rigidB == nullptr || rigidB->type() == E_BodyType::STATIC))
			this->setExit(_collisions[i]);
		if ((!colliderA->isTrigger() && !colliderB->isTrigger()) &&
			(rigidA == nullptr || rigidA->type() != E_BodyType::DYNAMIC) &&
			(rigidB == nullptr || rigidB->type() != E_BodyType::DYNAMIC))
			this->setExit(_collisions[i]);
		//bool activeA = rigidA != nullptr && rigidA->isAwake() && rigidA->type() != E_BodyType::STATIC;
		//bool activeB = rigidB != nullptr && rigidB->isAwake() && rigidB->type() != E_BodyType::STATIC;
		//if (!activeA && !activeB) continue; // TODO : 왜 삭제안하지??
		if (!broadPhase.getAABB(colliderA->key()).overlaps(broadPhase.getAABB(colliderB->key())))
			this->setExit(_collisions[i]);
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
	bool trigger = colliderA->isTrigger() || colliderB->isTrigger();
	E_GameObjectEvent& evt = collision->_evt;

	if (!gjk(collision))
	{
		if (evt == E_GameObjectEvent::NONE) return;
		this->setExit(collision);
	}
	else
	{
		if (trigger)
		{
			if (evt == E_GameObjectEvent::COLLISION_ENTER || evt == E_GameObjectEvent::COLLISION_STAY)
				this->setExit(collision);
			if (evt != E_GameObjectEvent::TRIGGER_ENTER && evt != E_GameObjectEvent::TRIGGER_STAY)
				evt = E_GameObjectEvent::TRIGGER_ENTER;
			else
				evt = E_GameObjectEvent::TRIGGER_STAY;
		}
		else
		{
			if (evt == E_GameObjectEvent::TRIGGER_ENTER || evt == E_GameObjectEvent::TRIGGER_STAY)
				this->setExit(collision);
			if (evt != E_GameObjectEvent::COLLISION_ENTER && evt != E_GameObjectEvent::COLLISION_STAY)
				evt = E_GameObjectEvent::COLLISION_ENTER;
			else
				evt = E_GameObjectEvent::COLLISION_STAY;
			this->epa(collision);
		}
	}
}
bool CollisionDetector::gjk(Collision2D* collision)
{
	Collider2D* colliderA = collision->colliderA();
	Collider2D* colliderB = collision->colliderB();
	Point2D pointA = colliderA->computeSupportPoint({ 1.0f,0.0f });
	Point2D pointB = colliderB->computeSupportPoint({ -1.0f,0.0f });

	collision->_simplex.init(pointA - pointB);
	while (true)
	{
		pointA = colliderA->computeSupportPoint(collision->_simplex.supportVec());
		pointB = colliderB->computeSupportPoint(collision->_simplex.supportVec() * -1);
		if (!collision->_simplex.insert(pointA - pointB)) return false;
		if (collision->_simplex.containsOrigin()) return true;
	}
}
void CollisionDetector::epa(Collision2D* collision)
{
	Polytope polytope(*collision, collision->_simplex.points());
	collision->_depth = polytope.depth();
	collision->_normal = polytope.normal();
}
void CollisionDetector::setExit(Collision2D* collision)
{
	Collider2D* colliderA = collision->colliderA();
	Collider2D* colliderB = collision->colliderB();
	GameObject* objA = colliderA->gameObject();
	GameObject* objB = colliderB->gameObject();
	E_GameObjectEvent& evt = collision->_evt;

	if (evt == E_GameObjectEvent::COLLISION_ENTER || evt == E_GameObjectEvent::COLLISION_STAY)
	{
		evt = E_GameObjectEvent::COLLISION_EXIT;
		objA->broadcast(evt, collision);
		objB->broadcast(evt, collision);
	}
	else if (evt == E_GameObjectEvent::TRIGGER_ENTER || evt == E_GameObjectEvent::TRIGGER_STAY)
	{
		evt = E_GameObjectEvent::TRIGGER_EXIT;
		objA->broadcast(evt, colliderB);
		objB->broadcast(evt, colliderA);
	}
}
std::vector<Collision2D*>::iterator CollisionDetector::remove(std::vector<Collision2D*>::iterator& it)
{
	Collision2D* collision = (*it);
	std::vector<Collision2D*>& collisionsA = collision->colliderA()->_collisions;
	std::vector<Collision2D*>& collisionsB = collision->colliderB()->_collisions;

	collisionsA.erase(std::remove(collisionsA.begin(), collisionsA.end(), collision), collisionsA.end());
	collisionsB.erase(std::remove(collisionsB.begin(), collisionsB.end(), collision), collisionsB.end());
	ObjectPool::release(collision);
	return _collisions.erase(it);
}
#pragma endregion
