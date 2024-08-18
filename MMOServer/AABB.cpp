#include "AABB.h"
#include <algorithm>

#pragma region public
int AABB::perimeter() const { return 2 * (_rightBtmX - _leftTopX + _rightBtmY - _leftTopY); }
bool AABB::overlaps(const AABB& obj) const
{
	if (_leftTopX >= obj._rightBtmX || _rightBtmX <= obj._leftTopX) return false;
	if (_leftTopY >= obj._rightBtmY || _rightBtmY <= obj._leftTopY) return false;
	return true;
}
AABB AABB::operator+(const AABB& obj) const
{
	int leftTopY = std::min(_leftTopY, obj._leftTopY);
	int leftTopX = std::min(_leftTopX, obj._leftTopX);
	int rightBtmY = std::max(_rightBtmY, obj._rightBtmY);
	int rightBtmX = std::max(_rightBtmX, obj._rightBtmX);
	return { leftTopY, leftTopX, rightBtmY, rightBtmX };
}
#pragma endregion

#pragma region private
#pragma endregion