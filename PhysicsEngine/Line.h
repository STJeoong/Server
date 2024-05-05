#pragma once
#include "Vector2D.h"
#include <utility>
class Line
{
public:
	Line(const Point2D& a, const Point2D& b);
	float distance(const Point2D& p) const;
private:

	Point2D _pass;
	bool _isPoint = false;
	// ax + by + c = 0
	float _a;
	float _b;
	float _c;
};