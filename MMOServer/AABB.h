#pragma once

class AABB
{
public:
	AABB() {}
	AABB(int minY, int minX, int maxY, int maxX) : _minY(minY), _minX(minX), _maxY(maxY), _maxX(maxX) {}
	int perimeter() const;
	bool overlaps(const AABB& obj) const;
	AABB operator+(const AABB& obj) const;
	void move(int deltaY, int deltaX);
private:

	int _minY = 0, _minX = 0, _maxY = 0, _maxX = 0;
};