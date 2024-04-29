#include "pch.h"
#include "CircleCollider.h"
#include "AABB.h"
#include "RigidBody2D.h"
#include "GameObject.h"

#pragma region public
CircleCollider* CircleCollider::clone() { return new CircleCollider(*this); }
AABB CircleCollider::computeAABB()
{
	const Point2D& worldObjPos = this->gameObject()->transform().position();
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	Point2D worldCenter = worldObjPos + worldObjRot * _offset;

	Vector2D v{ _radius, _radius };
	Point2D mini = worldCenter - v;
	Point2D maxi = worldCenter + v;
	return { mini, maxi };
}
#pragma endregion

#pragma region private
CircleCollider::CircleCollider(const S_CircleDef& def)
{
	_bounciness = Utils::clamp(def.bounciness, 100000.0f, 0.0f);
	_density = def.density;
	_friction = Utils::clamp(def.friction, 100000.0f, 0.0f);
	_isTrigger = def.isTrigger;
	_offset = def.offset;
	_radius = def.radius;

	if (_radius < MIN_SIZE) _radius = MIN_SIZE;
	this->compute();
}
void CircleCollider::compute()
{
	_mass = _density * PI * _radius * _radius;
	_inertia = _mass * (0.5f * _radius * _radius + _offset.squaredLen());
}
#pragma endregion