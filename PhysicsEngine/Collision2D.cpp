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
	if (!c1->isTrigger() && !c2->isTrigger() && _evt != E_GameObjectEvent::NONE)
	{
		if (c1->attachedRigidBody() != nullptr)
			c1->attachedRigidBody()->wakeUp();
		if (c2->attachedRigidBody() != nullptr)
			c2->attachedRigidBody()->wakeUp();
	}
	_contactA = {};
	_contactB = {};
	_colliderA = nullptr;
	_colliderB = nullptr;
	_normalImpulseSum = 0.0f;
	_tangentImpulseSum = 0.0f;
	_bounciness = 0.0f;
	_bouncinessThreshold = 1.0f;
	_friction = 0.0f;

	_evt = E_GameObjectEvent::NONE;
}