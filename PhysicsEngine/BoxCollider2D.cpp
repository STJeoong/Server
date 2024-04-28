#include "pch.h"
#include "BoxCollider2D.h"
#include "AABB.h"
#include "Matrix22.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "Utils.h"

#pragma region public
BoxCollider2D* BoxCollider2D::clone() { return new BoxCollider2D(*this); }
AABB BoxCollider2D::getAABB()
{
	const Point2D& worldObjPos = this->gameObject()->transform().position();
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	Point2D worldCenter = worldObjPos + worldObjRot * _offset;

	// before rotation
	Point2D beforeRot[4];
	beforeRot[0] = _halfSize;
	beforeRot[1] = { _halfSize.x() * -1, _halfSize.y() };
	beforeRot[2] = { _halfSize.x() * -1, _halfSize.y() * -1 };
	beforeRot[3] = { _halfSize.x(), _halfSize.y() * -1 };

	// after rotation. be careful! you have to add worldCenter after rotation.
	Point2D afterRot[4] = { worldObjRot * beforeRot[0], worldObjRot * beforeRot[1],
							worldObjRot * beforeRot[2], worldObjRot * beforeRot[3] };
	for (int i = 0; i < 4; ++i)
		afterRot[i] += worldCenter;

	Point2D mini = afterRot[0];
	Point2D maxi = afterRot[0];
	for (int i = 1; i < 4; ++i)
	{
		mini.x() = min(mini.x(), afterRot[i].x());
		mini.y() = min(mini.y(), afterRot[i].y());
		maxi.x() = max(maxi.x(), afterRot[i].x());
		maxi.y() = max(maxi.y(), afterRot[i].y());
	}
	return { mini, maxi };
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

	if (_halfSize.x() < MIN_SIZE) _halfSize.x() = MIN_SIZE;
	if (_halfSize.y() < MIN_SIZE) _halfSize.y() = MIN_SIZE;
	this->compute();
}
void BoxCollider2D::compute()
{
	Vector2D fullSize = _halfSize * 2;
	_mass = _density * fullSize.x() * fullSize.y();
	_inertia = _mass * (fullSize.x() * fullSize.x() + fullSize.y() * fullSize.y()) / 12.0f;
}
#pragma endregion
