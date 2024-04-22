#pragma once
#include "Vector2D.h"

class Collider2D;
struct Body
{
public:
	void integrate(float dt) { _velocity += _force * _invMass * dt; _position += _velocity * dt; }
	const Point2D& position() const { return _position; }
	const float& mass() const { return _mass; }
	const float& invMass() const { return _invMass; }
	Collider2D* collider() const { return _collider; }
	void position(const Point2D& p) { _position = p; }
	void mass(float m);
	void collider(Collider2D* c) { _collider = c; } // TODO : free
private:
	Point2D _position;
	Vector2D _velocity;
	Vector2D _force;
	float _mass = 1.0f;
	float _invMass = 1.0f;

	Collider2D* _collider = nullptr;
};

struct ArbiterKey
{
	ArbiterKey(Body* b1, Body* b2) { this->b1 = b1; this->b2 = b2; }
	bool operator<(const ArbiterKey& obj) { if (b1 < obj.b1) return true; if (b1 == obj.b1 && b2 < obj.b2) return true; return false; }
	Body* b1;
	Body* b2;
};
using ArbPair = std::pair<Body*, Body*>;