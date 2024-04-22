#include "pch.h"
#include "CollisionDetector.h"

#pragma region public
bool CollisionDetector::detect(const AABB& a, const AABB& b)
{
	if (a.maxi().x() < b.mini().x() || a.mini().x() > b.maxi().x()) return false;
	if (a.maxi().y() < b.mini().y() || a.mini().y() > b.maxi().y()) return false;
	return true;
}
#pragma endregion

#pragma region private
#pragma endregion