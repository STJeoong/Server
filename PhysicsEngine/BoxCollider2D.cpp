#include "pch.h"
#include "BoxCollider2D.h"
#include "AABB.h"
#include "Matrix22.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "Utils.h"
#include "Transform.h"

#pragma region public
BoxCollider2D* BoxCollider2D::clone() { return new BoxCollider2D(*this); }
#pragma endregion

#pragma region private
BoxCollider2D::BoxCollider2D(const S_BoxDef& def)
{
	_bounciness = Utils::clamp(def.bounciness, 100000.0f, 0.0f);
	_bouncinessThreshold = Utils::clamp(def.bouncinessThreshold, 100000.0f, 5.0f);
	_density = def.density;
	_friction = Utils::clamp(def.friction, 100000.0f, 0.0f);
	_halfSize = def.halfSize;
	_isTrigger = def.isTrigger;
	_offset = def.offset;

	if (_halfSize.x() < MIN_SIZE) _halfSize.x(MIN_SIZE);
	if (_halfSize.y() < MIN_SIZE) _halfSize.y(MIN_SIZE);
	this->compute();
}
AABB BoxCollider2D::computeAABB(const Transform& transform)
{
	Point2D points[4];
	this->computePoints(points, transform);
	Point2D mini = points[0];
	Point2D maxi = points[0];
	for (int i = 1; i < 4; ++i)
	{
		mini.x(std::min(mini.x(), points[i].x()));
		mini.y(std::min(mini.y(), points[i].y()));
		maxi.x(std::max(maxi.x(), points[i].x()));
		maxi.y(std::max(maxi.y(), points[i].y()));
	}
	return { mini, maxi };
}
Point2D BoxCollider2D::computeSupportPoint(const Vector2D& vec) const
{
	float maxVal = Vector2D::dot(vec, _points[0]);
	int maxIdx = 0;
	for (int i = 1; i < 4; ++i)
	{
		float val = Vector2D::dot(vec, _points[i]);
		if (val > maxVal)
		{
			maxVal = val;
			maxIdx = i;
		}
	}
	return _points[maxIdx];
}
Point2D BoxCollider2D::computeMarginSupportPoint(const Vector2D& vec) const
{
	float maxVal = Vector2D::dot(vec, _marginPoints[0]);
	int maxIdx = 0;
	for (int i = 1; i < 4; ++i)
	{
		float val = Vector2D::dot(vec, _marginPoints[i]);
		if (val > maxVal)
		{
			maxVal = val;
			maxIdx = i;
		}
	}
	return _marginPoints[maxIdx];
}
bool BoxCollider2D::perpendicularToTheSide(const Vector2D& vec) const
{
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	Vector2D rotated = worldObjRot.transpose() * vec;

	if (rotated == Vector2D(1.0f, 0.0f))
		return true;
	if (rotated == Vector2D(-1.0f, 0.0f))
		return true;
	if (rotated == Vector2D(0.0f, 1.0f))
		return true;
	if (rotated == Vector2D(0.0f, -1.0f))
		return true;
	return false;
}
bool BoxCollider2D::project(const Vector2D& oldNormal, bool isA, const Point2D& otherGlobalPoint, const Point2D& myGlobalPoint, Point2D localEdgePoints[2])
{
	static const float TOLERANCE = 0.0001f;
	Point2D otherLocalPoint = this->toLocal(otherGlobalPoint);
	Point2D myLocalPoint = this->toLocal(myGlobalPoint);
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	Vector2D localNormal = worldObjRot.transpose() * oldNormal;

	if ((otherLocalPoint.x() < -_halfSize.x() - TOLERANCE || otherLocalPoint.x() > _halfSize.x() + TOLERANCE) &&
		(otherLocalPoint.y() < -_halfSize.y() - TOLERANCE || otherLocalPoint.y() > _halfSize.y() + TOLERANCE))
		return false;

	Point2D localPoints[4];
	localPoints[0] = _halfSize;
	localPoints[1] = { _halfSize.x() * -1, _halfSize.y() };
	localPoints[2] = { _halfSize.x() * -1, _halfSize.y() * -1 };
	localPoints[3] = { _halfSize.x(), _halfSize.y() * -1 };

	bool horizontal = -_halfSize.x() - TOLERANCE <= otherLocalPoint.x() && otherLocalPoint.x() <= _halfSize.x() + TOLERANCE;
	if (myLocalPoint == localPoints[0])
	{
		localEdgePoints[0] = localPoints[0];
		if (isA)
		{
			if (localNormal.y() > localNormal.x())
				localEdgePoints[1] = localPoints[1];
			else
				localEdgePoints[1] = localPoints[3];
		}
		else
		{
			if (localNormal.y() < localNormal.x())
				localEdgePoints[1] = localPoints[1];
			else
				localEdgePoints[1] = localPoints[3];
		}
	}
	else if (myLocalPoint == localPoints[1])
	{
		localEdgePoints[0] = localPoints[1];
		if (isA)
		{
			if (localNormal.y() > -localNormal.x())
				localEdgePoints[1] = localPoints[0];
			else
				localEdgePoints[1] = localPoints[2];
		}
		else
		{
			if (localNormal.y() < -localNormal.x())
				localEdgePoints[1] = localPoints[0];
			else
				localEdgePoints[1] = localPoints[2];
		}
	}
	else if (myLocalPoint == localPoints[2])
	{
		localEdgePoints[0] = localPoints[2];
		if (isA)
		{
			if (-localNormal.y() > -localNormal.x())
				localEdgePoints[1] = localPoints[3];
			else
				localEdgePoints[1] = localPoints[1];
		}
		else
		{
			if (-localNormal.y() < -localNormal.x())
				localEdgePoints[1] = localPoints[3];
			else
				localEdgePoints[1] = localPoints[1];
		}
	}
	else if (myLocalPoint == localPoints[3])
	{
		localEdgePoints[0] = localPoints[3];
		if (isA)
		{
			if (-localNormal.y() > localNormal.x())
				localEdgePoints[1] = localPoints[2];
			else
				localEdgePoints[1] = localPoints[0];
		}
		else
		{
			if (-localNormal.y() < localNormal.x())
				localEdgePoints[1] = localPoints[2];
			else
				localEdgePoints[1] = localPoints[0];
		}
	}
	return true;
}
bool BoxCollider2D::containsPoint(const Point2D& p)
{
	static const float TOLERANCE = 0.0001f;
	Point2D localPoint = this->toLocal(p);

	return (-_halfSize.x() - TOLERANCE < p.x() && p.x() < _halfSize.x() + TOLERANCE) &&
		(-_halfSize.y() - TOLERANCE < p.y() && p.y() < _halfSize.y() + TOLERANCE);
}
void BoxCollider2D::updatePoints()
{
	this->computePoints(_points, this->gameObject()->transform());
	this->computeMarginPoints(_marginPoints, this->gameObject()->transform());
}
void BoxCollider2D::compute()
{
	Vector2D fullSize = _halfSize * 2;
	_mass = _density * fullSize.x() * fullSize.y();
	_inertia = _mass * (fullSize.x() * fullSize.x() + fullSize.y() * fullSize.y()) / 12.0f;
}
void BoxCollider2D::computePoints(Point2D points[4], const Transform& transform) const
{
	// before rotation
	Point2D beforeRot[4];
	beforeRot[0] = _halfSize;
	beforeRot[1] = { _halfSize.x() * -1, _halfSize.y() };
	beforeRot[2] = { _halfSize.x() * -1, _halfSize.y() * -1 };
	beforeRot[3] = { _halfSize.x(), _halfSize.y() * -1 };

	// after rotation. be careful! you have to add worldCenter after rotation.
	for (int i = 0; i < 4; ++i)
		points[i] = transform.rotation() * beforeRot[i];
	for (int i = 0; i < 4; ++i)
		points[i] += transform.position();
}
void BoxCollider2D::computeMarginPoints(Point2D points[4], const Transform& transform) const
{
	// before rotation
	Point2D beforeRot[4];
	Vector2D marginSize = _halfSize + Vector2D(COLLIDER_MARGIN, COLLIDER_MARGIN);
	beforeRot[0] = marginSize;
	beforeRot[1] = { marginSize.x() * -1, marginSize.y() };
	beforeRot[2] = { marginSize.x() * -1, marginSize.y() * -1 };
	beforeRot[3] = { marginSize.x(), marginSize.y() * -1 };

	// after rotation. be careful! you have to add worldCenter after rotation.
	for (int i = 0; i < 4; ++i)
		points[i] = transform.rotation() * beforeRot[i];
	for (int i = 0; i < 4; ++i)
		points[i] += transform.position();
}
#pragma endregion
