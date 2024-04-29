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
			std::vector<Contact2D*>& contactsA = (*it)->_colliderA->_contacts;
			std::vector<Contact2D*>& contactsB = (*it)->_colliderB->_contacts;
			contactsA.erase(std::remove_if(contactsA.begin(), contactsA.end(),
				[&collider](Contact2D* c) { return c->_colliderA == collider || c->_colliderB == collider; }), contactsA.end());
			contactsB.erase(std::remove_if(contactsB.begin(), contactsB.end(),
				[&collider](Contact2D* c) { return c->_colliderA == collider || c->_colliderB == collider; }), contactsB.end());
			ObjectPool::release(*it);
			it = _collisions.erase(it);
		}
	}
}
void CollisionDetector::update(const BroadPhase& broadPhase)
{
	// update new things.
	const std::vector<std::pair<int, int>>& pairs = broadPhase.candidates();
	for (int i = 0; i < pairs.size(); ++i)
	{
		Collider2D* colliderA = reinterpret_cast<Collider2D*>(broadPhase.getData(pairs[i].first));
		Collider2D* colliderB = reinterpret_cast<Collider2D*>(broadPhase.getData(pairs[i].second));
		RigidBody2D* rigidA = colliderA->attachedRigidBody();
		RigidBody2D* rigidB = colliderB->attachedRigidBody();

		if (rigidA == rigidB) continue;
		if (!ContactFilter::shouldCollide(*colliderA, *colliderB)) continue;
		if ((rigidA == nullptr || rigidA->type() == E_BodyType::STATIC) && ((rigidB == nullptr || rigidB->type() == E_BodyType::STATIC))) continue;
		auto it = std::find_if(colliderA->contacts().begin(), colliderA->contacts().end(),
			[&colliderB](Contact2D* c) { return c->colliderA() == colliderB || c->colliderB() == colliderB; });
		if (it != colliderA->contacts().end()) continue; // already exist

		Collision2D* collision = ObjectPool::get<Collision2D>();
	}

	// update old things.
}
#pragma endregion

#pragma region private
#pragma endregion
