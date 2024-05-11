#include "pch.h"
#include "Collision2D.h"
#include "Collider2D.h"
#include "RigidBody2D.h"
#include <ObjectPool.h>
#include "Contact2D.h"

#pragma region public
#pragma endregion

#pragma region private
void Collision2D::validateOldContacts()
{
	auto it = _contacts.begin();
	while (it != _contacts.end())
	{
		if (!this->isValid(*it))
		{
			ObjectPool::release(*it);
			it = _contacts.erase(it);
		}
		else
			++it;
	}
}
bool Collision2D::importNewContact(Contact2D* contact)
{
	for (Contact2D* c : _contacts)
	{
		bool farEnoughA = (contact->contactA() - c->contactA()).squaredLen() > Collision2D::PERSISTENT_THRESHOLD_SQUARED;
		bool farEnoughB = (contact->contactB() - c->contactB()).squaredLen() > Collision2D::PERSISTENT_THRESHOLD_SQUARED;
		if (!farEnoughA || !farEnoughB) // TODO : ¸Â´ÂÁö
			return false;
	}
	_contacts.push_back(contact);
	return true;
}
bool Collision2D::isValid(Contact2D* contact)
{
	Point2D globalPointA = _colliderA->toWorld(contact->localContactA());
	Point2D globalPointB = _colliderB->toWorld(contact->localContactB());

	if (Vector2D::dot(globalPointB - globalPointA, contact->normal()) > 0.0f)
		return false;
	bool closeEnoughA = (contact->contactA() - globalPointA).squaredLen() < Collision2D::PERSISTENT_THRESHOLD_SQUARED;
	bool closeEounghB = (contact->contactB() - globalPointB).squaredLen() < Collision2D::PERSISTENT_THRESHOLD_SQUARED;
	if (!closeEnoughA || !closeEounghB)
		return false;
	contact->_contactA = globalPointA;
	contact->_contactB = globalPointB;
	contact->_depth = Vector2D::dot(globalPointA - globalPointB, contact->normal());
	contact->_rA = globalPointA - contact->colliderA()->position();
	contact->_rB = globalPointB - contact->colliderB()->position();
	return true;
}
void Collision2D::onDestroy()
{
	Collider2D* c1 = _colliderA;
	Collider2D* c2 = _colliderB;
	if (!c1->isTrigger() && !c2->isTrigger() && _evt != E_GameObjectEvent::NONE)
	{
		if (c1->attachedRigidBody() != nullptr)
			c1->attachedRigidBody()->wakeUp();
		if (c2->attachedRigidBody() != nullptr)
			c2->attachedRigidBody()->wakeUp();
	}
	for (Contact2D* c : _contacts)
		ObjectPool::release(c);
	_contacts.clear();
	_colliderA = nullptr;
	_colliderB = nullptr;
	_bounciness = 0.0f;
	_bouncinessThreshold = 1.0f;
	_friction = 0.0f;

	_evt = E_GameObjectEvent::NONE;

	_bouncinessBias = 0.0f;
}
#pragma endregion

const float Collision2D::PERSISTENT_THRESHOLD_SQUARED = 0.01f;