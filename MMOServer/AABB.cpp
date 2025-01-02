#include "AABB.h"
#include <algorithm>

#pragma region public
int AABB::perimeter() const { return 2 * (_maxX - _minX + _maxY - _minY); }
bool AABB::overlaps(const AABB& obj) const
{
	if (_minX >= obj._maxX || _maxX <= obj._minX) return false;
	if (_minY >= obj._maxY || _maxY <= obj._minY) return false;
	return true;
}
AABB AABB::operator+(const AABB& obj) const
{
	int minY = std::min(_minY, obj._minY);
	int minX = std::min(_minX, obj._minX);
	int maxY = std::max(_maxY, obj._maxY);
	int maxX = std::max(_maxX, obj._maxX);
	return { minY, minX, maxY, maxX };
}
void AABB::move(int deltaY, int deltaX) { _minY += deltaY; _minX += deltaX; _maxY += deltaY; _maxX += deltaX; }
#pragma endregion

#pragma region private
#pragma endregion