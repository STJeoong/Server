#include "pch.h"
#include "BoxCollider2D.h"
#include "AABB.h"
#include "Matrix22.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "Utils.h"

#pragma region public
BoxCollider2D* BoxCollider2D::clone() { return new BoxCollider2D(*this); }
AABB BoxCollider2D::computeAABB()
{
	this->computePoints();

	Point2D mini = _points[0];
	Point2D maxi = _points[0];
	for (int i = 1; i < 4; ++i)
	{
		mini.x(std::min(mini.x(), _points[i].x()));
		mini.y(std::min(mini.y(), _points[i].y()));
		maxi.x(std::max(maxi.x(), _points[i].x()));
		maxi.y(std::max(maxi.y(), _points[i].y()));
	}
	return { mini, maxi };
}
Point2D BoxCollider2D::computeSupportPoint(const Vector2D& vec)
{
	this->computePoints();
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
#pragma endregion

#pragma region private
BoxCollider2D::BoxCollider2D(const S_BoxDef& def)
{
	_bounciness = Utils::clamp(def.bounciness, 100000.0f, 0.0f);
	_density = def.density;
	_friction = Utils::clamp(def.friction, 100000.0f, 0.0f);
	_halfSize = def.halfSize;
	_isTrigger = def.isTrigger;
	_offset = def.offset;

	if (_halfSize.x() < MIN_SIZE) _halfSize.x(MIN_SIZE);
	if (_halfSize.y() < MIN_SIZE) _halfSize.y(MIN_SIZE);
	_size = _halfSize.len();
	this->compute();
}
void BoxCollider2D::compute()
{
	Vector2D fullSize = _halfSize * 2;
	_mass = _density * fullSize.x() * fullSize.y();
	_inertia = _mass * (fullSize.x() * fullSize.x() + fullSize.y() * fullSize.y()) / 12.0f;
}
void BoxCollider2D::computePoints()
{
	Point2D worldCenter = this->position();
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();

	// before rotation
	Point2D beforeRot[4];
	beforeRot[0] = _halfSize;
	beforeRot[1] = { _halfSize.x() * -1, _halfSize.y() };
	beforeRot[2] = { _halfSize.x() * -1, _halfSize.y() * -1 };
	beforeRot[3] = { _halfSize.x(), _halfSize.y() * -1 };

	// after rotation. be careful! you have to add worldCenter after rotation.
	_points = { worldObjRot * beforeRot[0], worldObjRot * beforeRot[1],
				worldObjRot * beforeRot[2], worldObjRot * beforeRot[3] };
	for (int i = 0; i < 4; ++i)
		_points[i] += worldCenter;
}
#pragma endregion
