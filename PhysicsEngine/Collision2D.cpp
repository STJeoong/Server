#include "pch.h"
#include "Collision2D.h"
#include "Collider2D.h"
#include <ObjectPool.h>
#include "Contact2D.h"
#include "RigidBody2D.h"
#include "Line.h"

#include "GameObject.h"

#pragma region public
#pragma endregion

#pragma region private
void Collision2D::validateOldContacts()
{
	auto it = _contacts.begin();
	while (it != _contacts.end())
	{
		Contact2D* contact = *it;
		if (contact->depth() < 0.0f)
			contact->_normalImpulse = contact->_tangentImpulse = 0.0f;
		if (contact->depth() < -2.0f * COLLIDER_MARGIN)
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
	static const float PERSISTENT_SQUARED = 0.000001f;
	//원은 contact 1개만 가질 수 있음.
	if ((contact->_isCircleA || contact->_isCircleB) && _contacts.size() == 1)
	{
		// 기존 정보와 동일한 건 pass
		if (contact->_isCircleA == _contacts[0]->_isCircleA && contact->_isCircleB && _contacts[0]->_isCircleB &&
			contact->_isEdgeA == _contacts[0]->_isEdgeA && contact->_isEdgeB && _contacts[0]->_isEdgeB)
			return false;
		ObjectPool::release(_contacts[0]);
		_contacts.erase(_contacts.begin());
	}

	for (Contact2D* c : _contacts)
	{
		if (*contact == *c)
		{
			float nImpulse = c->_normalImpulse;
			float tImpulse = c->_tangentImpulse;
			*c = *contact;
			c->_normalImpulse = nImpulse;
			c->_tangentImpulse = tImpulse;
			return false;
		}
		bool farEnoughA = (contact->contactA() - c->contactA()).squaredLen() > PERSISTENT_SQUARED;
		bool farEnoughB = (contact->contactB() - c->contactB()).squaredLen() > PERSISTENT_SQUARED;
		bool closeEnoughA = (contact->contactA() - c->contactA()).squaredLen() < PERSISTENT_SQUARED;
		bool closeEnoughB = (contact->contactB() - c->contactB()).squaredLen() < PERSISTENT_SQUARED;
		if (closeEnoughA && closeEnoughB)
		{
			float nImpulse = c->_normalImpulse;
			float tImpulse = c->_tangentImpulse;
			*c = *contact;
			c->_normalImpulse = nImpulse;
			c->_tangentImpulse = tImpulse;
			return false;
		}
		if (!farEnoughA || !farEnoughB)
			return false;
	}
	_contacts.push_back(contact);
	if (_contacts.size() > 2)
		this->prune();
	return true;
}
void Collision2D::prune()
{
	Contact2D* deepest = _contacts[2];

	Contact2D* farthestFromDeepest = _contacts[0];
	float distanceSq = (farthestFromDeepest->contactA() - deepest->contactA()).squaredLen();
	for (int i = 1; i < _contacts.size(); ++i)
	{
		float dist = (_contacts[i]->contactA() - deepest->contactA()).squaredLen();
		if (dist > distanceSq)
		{
			distanceSq = dist;
			farthestFromDeepest = _contacts[i];
		}
	}

	auto it = std::find_if(_contacts.begin(), _contacts.end(),
		[deepest, farthestFromDeepest](Contact2D* c) { return c != deepest && c != farthestFromDeepest; });
	ObjectPool::release(*it);
	_contacts.erase(it);
}
void Collision2D::updateContacts()
{
	for (Contact2D* c : _contacts)
		c->update();
}
void Collision2D::computeBouncinessBias()
{
	RigidBody2D* rigidA = _colliderA->attachedRigidBody();
	RigidBody2D* rigidB = _colliderB->attachedRigidBody();

	for (int i = 0; i < _contacts.size(); ++i)
	{
		Contact2D& contact = *(_contacts[i]);
		Vector2D relativeVel = {};
		float normalVel = 0.0f;

		if (rigidA != nullptr && rigidA->type() != E_BodyType::STATIC)
			relativeVel -= rigidA->velocity() + Vector2D::cross(rigidA->angularVelocity(), contact.rA());
		if (rigidB != nullptr && rigidB->type() != E_BodyType::STATIC)
			relativeVel += rigidB->velocity() + Vector2D::cross(rigidB->angularVelocity(), contact.rB());
		normalVel = Vector2D::dot(relativeVel, contact.normal());

		if (-normalVel > _bouncinessThreshold)
			contact._bouncinessBias = _bounciness * normalVel;
		else
			contact._bouncinessBias = 0.0f;
	}
}
void Collision2D::clearContacts()
{
	auto it = _contacts.begin();
	while (it != _contacts.end())
	{
		ObjectPool::release(*it);
		it = _contacts.erase(it);
	}
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
	_evt = E_GameObjectEvent::NONE;
}
#pragma endregion