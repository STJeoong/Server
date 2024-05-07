#pragma once
#include <vector>
#include "Vector2D.h"
class Simplex
{
public:
	void init(const Point2D& pointFromA, const Point2D& pointFromB);
	// @return check if simplex is possible to contain the origin.
	bool insert(const Point2D& pointFromA, const Point2D& pointFromB);
	const Vector2D& supportVec() const { return _supportVec; }
	bool containsOrigin() const { return _containsOrigin; }
	const std::vector<Point2D>& points() const { return _points; }
	const std::vector<std::pair<Point2D, Point2D>>& sources() const { return _sources; }
private:
	// 충돌가능성 체크
	bool check();

	std::vector<Point2D> _points;
	std::vector<std::pair<Point2D, Point2D>> _sources;
	Vector2D _supportVec; // next support function's direction
	bool _containsOrigin = false;
};