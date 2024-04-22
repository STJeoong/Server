#include "pch.h"
#include "Circle.h"
#include "AABB.h"

#pragma region public
AABB Circle::getAABB()
{
	Vector2D v{ _radius, _radius };
	Point2D mini = _center - v;
	Point2D maxi = _center + v;
	return { mini, maxi };
}
#pragma endregion

#pragma region private
#pragma endregion