#pragma once
#include "Vector2D.h"
class Simplex
{
public:
	void init(const Point2D& p);
	// @return check if simplex is possible to contain the origin.
	bool insert(const Point2D& p);
	const Vector2D& supportVec() const { return _supportVec; }
	bool containsOrigin() const { return _containsOrigin; }
private:
	// 충돌가능성 체크
	bool check();

	Point2D _points[3];
	Vector2D _supportVec; // next support function's direction
	int _state = 0; // n-simplex
	bool _containsOrigin = false;
};