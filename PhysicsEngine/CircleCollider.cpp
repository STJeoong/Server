#include "pch.h"
#include "CircleCollider.h"
#include "AABB.h"
#include "RigidBody2D.h"

#pragma region public
CircleCollider* CircleCollider::clone() { return new CircleCollider(*this); }
void CircleCollider::onAddComponent(Component* component)
{
	RigidBody2D* rigid = dynamic_cast<RigidBody2D*>(component);
	if (rigid == nullptr) return;
	this->attachTo(rigid);
}
AABB CircleCollider::getAABB()
{
	Vector2D v{ _radius, _radius };
	Point2D mini = _offset - v;
	Point2D maxi = _offset + v;
	return { mini, maxi };
}
#pragma endregion

#pragma region private
CircleCollider::CircleCollider(const S_CircleDef& def)
{
	_bounciness = def.bounciness;
	_density = def.density;
	_friction = def.friction;
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