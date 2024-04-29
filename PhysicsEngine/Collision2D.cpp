#include "pch.h"
#include "Collision2D.h"
#include "Collider2D.h"
#include "RigidBody2D.h"
#include <ObjectPool.h>

#pragma region public
#pragma endregion

#pragma region private
#pragma endregion

void Collision2D::onDestroy()
{
	Collider2D* c1 = _colliderA;
	Collider2D* c2 = _colliderB;
	if (this->contactCount() > 0 && !c1->isTrigger() && !c2->isTrigger())
	{
		c1->attachedRigidBody()->wakeUp();
		c2->attachedRigidBody()->wakeUp();
	}
	for (int i = 0; i < _contacts.size(); ++i)
		ObjectPool::release(_contacts[i]);
	_contacts.clear();
}