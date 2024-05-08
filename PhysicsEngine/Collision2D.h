#pragma once
#include <vector>
#include "E_GameObjectEvent.h"
#include "Simplex.h"
class Collider2D;
class RigidBody2D;
class Collision2D
{
	friend class World;
	friend class CollisionDetector;
	friend class Solver;
public:
	Collider2D* const& colliderA() const { return _colliderA; }
	Collider2D* const& colliderB() const { return _colliderB; }
	const Vector2D& normal() const { return _normal; }
	const Vector2D& tangent() const { return _tangent; }
private:
	void onDestroy();

	Point2D _contactA;
	Point2D _contactB;
	Vector2D _rA; // from center of A to contactA
	Vector2D _rB; // from center of B to contactB
	Point2D _localA; // local position of contactA
	Point2D _localB; // local position of contactB

	Collider2D* _colliderA = nullptr;
	Collider2D* _colliderB = nullptr;
	float _bounciness = 0.0f;
	float _bouncinessThreshold = 1.0f;
	float _friction = 0.0f;
	bool _isTrigger = false;
	Vector2D _normal;
	Vector2D _tangent;
	float _normalImpulseSum = 0.0f;
	float _tangentImpulseSum = 0.0f;

	Simplex _simplex;
	E_GameObjectEvent _evt = E_GameObjectEvent::NONE;
};