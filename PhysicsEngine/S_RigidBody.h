#pragma once
#include "Vector2D.h"
struct S_RigidBody
{
	float mass;
	float inertia;
	Vector2D pos;
	Vector2D velocity;
	float angVelocity;
	float invMass;
	float invInertia;
};