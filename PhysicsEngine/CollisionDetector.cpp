#include "pch.h"
#include "CollisionDetector.h"
#include "BroadPhase.h"
#include "Collider2D.h"
#include "ContactFilter.h"
#include "RigidBody2D.h"
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
		{
			this->remove(_collisions[i]);
			continue;
		}
		if ((rigidA == nullptr || rigidA->type() != E_BodyType::DYNAMIC) && (rigidB == nullptr || rigidB->type() != E_BodyType::DYNAMIC))
		{
			this->remove(_collisions[i]);
			continue;
		}
		bool activeA = rigidA != nullptr && rigidA->isAwake() && rigidA->type() != E_BodyType::STATIC;
		bool activeB = rigidB != nullptr && rigidB->isAwake() && rigidB->type() != E_BodyType::STATIC;
		if (!activeA && !activeB) continue; // TODO : 왜 삭제안하지??
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

		auto it = std::find_if(colliderA->contacts().begin(), colliderA->contacts().end(),
			[&colliderB](Contact2D* c) { return c->colliderA() == colliderB || c->colliderB() == colliderB; });
		if (rigidA == rigidB) continue;
		if (!ContactFilter::shouldCollide(*colliderA, *colliderB)) continue;
		if ((rigidA == nullptr || rigidA->type() != E_BodyType::DYNAMIC) && (rigidB == nullptr || rigidB->type() != E_BodyType::DYNAMIC)) continue;
		if (it != colliderA->contacts().end()) continue; // already exist

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
	bool wasTouching = collision->_isTouching;
	bool sensor = colliderA->isTrigger() || colliderB->isTrigger();
	if (sensor)
	{

	}
	else
	{

	}
}
void CollisionDetector::remove(Collision2D* collision)
{
	std::vector<Contact2D*>& contactsA = collision->colliderA()->_contacts;
	std::vector<Contact2D*>& contactsB = collision->colliderB()->_contacts;
	contactsA.erase(std::remove_if(contactsA.begin(), contactsA.end(),
		[&collision](Contact2D* c) { return (c->colliderA() == collision->colliderA() && c->colliderB() == collision->colliderB()) ||
											(c->colliderA() == collision->colliderB() && c->colliderB() == collision->colliderA()); }), contactsA.end());
	contactsB.erase(std::remove_if(contactsB.begin(), contactsB.end(),
		[&collision](Contact2D* c) { return (c->colliderA() == collision->colliderA() && c->colliderB() == collision->colliderB()) ||
											(c->colliderA() == collision->colliderB() && c->colliderB() == collision->colliderA()); }), contactsB.end());
	ObjectPool::release(collision);
}
#pragma endregion

#pragma region private
#pragma endregion
