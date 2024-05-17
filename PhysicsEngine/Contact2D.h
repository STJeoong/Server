#pragma once
#include "Vector2D.h"

class Collider2D;
class Contact2D
{
	friend class CollisionDetector;
	friend class Solver;
	friend class World;

	// test
	friend class Collision2D;
public:
	const Point2D& contactA() const { return _contactA; }
	const Point2D& contactB() const { return _contactB; }
	const Point2D& localContactA() const { return _localContactA; }
	const Point2D& localContactB() const { return _localContactB; }
	Collider2D* colliderA() const { return _colliderA; }
	Collider2D* colliderB() const { return _colliderB; }
	const Vector2D& normal() const { return _normal; }
	const Vector2D& tangent() const { return _tangent; }
	float depth() const { return _depth; }
	float normalImpulse() const { return _normalImpulse; }
	float tangentImpulse() const { return _tangentImpulse; }
private:
	void onDestroy();

	Point2D _contactA;
	Point2D _contactB;
	Point2D _localContactA; // local position of contactA
	Point2D _localContactB; // local position of contactB
	Vector2D _rA;
	Vector2D _rB;
	Collider2D* _colliderA = nullptr;
	Collider2D* _colliderB = nullptr;
	Vector2D _normal;
	Vector2D _tangent;
	float _normalImpulse = 0.0f;
	float _tangentImpulse = 0.0f;
	float _depth = 0.0f;

	bool _isEdgeA;
	bool _isEdgeB;
	float _rotationA = 0.0f;
	float _rotationB = 0.0f;

	bool _isUsed = true;
	
	float _bouncinessBias = 0.0f;
};