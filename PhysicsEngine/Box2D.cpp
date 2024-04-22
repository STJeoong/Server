#include "pch.h"
#include "Box2D.h"
#include "AABB.h"
#include "Matrix22.h"

AABB Box2D::getAABB()
{
	Matrix22 r(_rotation);

	// before rotation
	Point2D beforeRot[4];
	beforeRot[0] = _halfSize;
	beforeRot[1] = { _halfSize.x() * -1, _halfSize.y() };
	beforeRot[2] = { _halfSize.x() * -1, _halfSize.y() * -1 };
	beforeRot[3] = { _halfSize.x(), _halfSize.y() * -1 };

	// after rotation
	Point2D afterRot[4] = { r * beforeRot[0], r * beforeRot[1], r * beforeRot[2], r * beforeRot[3] };

	Point2D mini = afterRot[0];
	Point2D maxi = afterRot[0];
	for (int i = 1; i < 4; ++i)
	{
		mini.x(min(mini.x(), afterRot[i].x()));
		mini.y(min(mini.y(), afterRot[i].y()));
		maxi.x(max(maxi.x(), afterRot[i].x()));
		maxi.y(max(maxi.y(), afterRot[i].y()));
	}
	return { mini, maxi };
}