#include "pch.h"
#include "Collider2D.h"
#include "RigidBody2D.h"

#pragma region public
#pragma endregion

#pragma region protected
void Collider2D::attachTo(RigidBody2D* rigid) { rigid->addCollider(this); }
#pragma endregion