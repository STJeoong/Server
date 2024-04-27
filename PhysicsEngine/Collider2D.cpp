#include "pch.h"
#include "Collider2D.h"
#include "RigidBody2D.h"

#pragma region public
#pragma endregion

#pragma region protected
#pragma endregion

#pragma region private
void Collider2D::onAddComponent(Component* component)
{
	RigidBody2D* rigid = dynamic_cast<RigidBody2D*>(component);
	if (rigid == nullptr) return;
	rigid->addCollider(this);
}
#pragma endregion