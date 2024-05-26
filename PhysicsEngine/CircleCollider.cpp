#include "pch.h"
#include "CircleCollider.h"
#include "AABB.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "Transform.h"

#pragma region public
CircleCollider* CircleCollider::clone() { return new CircleCollider(*this); }
#pragma endregion

#pragma region private
CircleCollider::CircleCollider(const S_CircleDef& def)
{
	_bounciness = Utils::clamp(def.bounciness, 100000.0f, 0.0f);
	_bouncinessThreshold = Utils::clamp(def.bouncinessThreshold, 100000.0f, 5.0f);
	_density = def.density;
	_friction = Utils::clamp(def.friction, 100000.0f, 0.0f);
	_isTrigger = def.isTrigger;
	_offset = def.offset;
	_radius = def.radius;

	if (_radius < MIN_SIZE) _radius = MIN_SIZE;
	this->compute();
}
AABB CircleCollider::computeAABB(const Transform& transform)
{
	Point2D worldCenter = transform.position() + transform.rotation() * _offset;
	Vector2D v{ _radius, _radius };
	Point2D mini = worldCenter - v;
	Point2D maxi = worldCenter + v;

	return { mini, maxi };
}
Point2D CircleCollider::computeSupportPoint(const Vector2D& vec) const
{
	Point2D worldCenter = this->position();
	Vector2D dirVec = vec.normalized();

	return worldCenter + dirVec * _radius;
}
Point2D CircleCollider::computeMarginSupportPoint(const Vector2D& vec) const
{
	Point2D worldCenter = this->position();
	Vector2D dirVec = vec.normalized();

	return worldCenter + dirVec * (_radius + COLLIDER_MARGIN);
}
bool CircleCollider::perpendicularToTheSide(const Vector2D& vec) const
{
	return false;
}
bool CircleCollider::project(const Vector2D& oldNormal, bool isA, const Point2D& otherGlobalPoint, const Point2D& myGlobalPoint, Point2D localEdgePoints[2])
{
	Vector2D dir = (otherGlobalPoint - myGlobalPoint).normalized();
	localEdgePoints[0] = localEdgePoints[1] = dir * _radius;
	return true;
}
void CircleCollider::compute()
{
	_mass = _density * PI * _radius * _radius;
	_inertia = _mass * (0.5f * _radius * _radius + _offset.squaredLen());
}
#pragma endregion