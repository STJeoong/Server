#pragma once
#include "Vector2D.h"

class Collider2D;
class Contact2D
{
	friend class World;
	friend class Solver;
	friend class Collision2D;
	friend class ContactGenerator;
public:
	const Point2D& contactA() const { return _contactA; }
	const Point2D& contactB() const { return _contactB; }
	Collider2D* colliderA() const { return _colliderA; }
	Collider2D* colliderB() const { return _colliderB; }
	const Vector2D& normal() const { return _normal; }
	const Vector2D& tangent() const { return _tangent; }
	float depth() const { return _depth; }
	float normalImpulse() const { return _normalImpulse; }
	float tangentImpulse() const { return _tangentImpulse; }
	const Vector2D& rA() const { return _rA; }
	const Vector2D& rB() const { return _rB; }
	float bouncinessBias() const { return _bouncinessBias; }
private:
	void update(); // update depth, normal, tangent, rA, rB
	void processCircle();
	bool changeEdge(const Point2D& p, Point2D globalPoints[2]);
	int getCloserPoint(const Point2D& p, Point2D globalPoints[2]);
	void onDestroy();
	bool operator==(const Contact2D& other);

	Point2D _contactA;
	Point2D _contactB;
	Vector2D _rA;
	Vector2D _rB;
	Collider2D* _colliderA = nullptr;
	Collider2D* _colliderB = nullptr;
	Vector2D _localNormal;
	Vector2D _normal;
	Vector2D _tangent;
	float _normalImpulse = 0.0f;
	float _tangentImpulse = 0.0f;
	float _depth = 0.0f;

	// for update
	bool _isEdgeA;
	bool _isEdgeB;
	bool _isCircleA;
	bool _isCircleB;
	Point2D _localEdgePointsA[2];
	Point2D _localEdgePointsB[2];
	
	float _bouncinessBias = 0.0f;
};