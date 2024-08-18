#pragma once

class AABB
{
public:
	AABB() {}
	AABB(int leftTopY, int leftTopX, int rightBtmY, int rightBtmX) : _leftTopY(leftTopY), _leftTopX(leftTopX), _rightBtmY(rightBtmY), _rightBtmX(rightBtmX) {}
	int perimeter() const;
	bool overlaps(const AABB& obj) const;
	AABB operator+(const AABB& obj) const;
private:

	int _leftTopY = 0, _leftTopX = 0, _rightBtmY = 0, _rightBtmX = 0;
};