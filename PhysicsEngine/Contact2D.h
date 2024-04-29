#pragma once
#include "Vector2D.h"

class Collider2D;
class RigidBody2D;
class Contact2D
{
	friend class CollisionDetector;
public:
	const Point2D& point() const { return _point; }
	const Vector2D& normal() const { return _normal; }
	Collider2D* const& colliderA() const { return _colliderA; }
	Collider2D* const& colliderB() const { return _colliderB; }
	RigidBody2D* const& rigidA() const { return _rigidA; }
	RigidBody2D* const& rigidB() const { return _rigidB; }
private:

	Point2D _point;
	Vector2D _normal;
	Collider2D* _colliderA = nullptr;
	Collider2D* _colliderB = nullptr;
	RigidBody2D* _rigidA = nullptr;
	RigidBody2D* _rigidB = nullptr;
};