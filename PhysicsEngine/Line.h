#pragma once
#include "Vector2D.h"

// ax + by + c = 0
class Line
{
public:
	Line(const Point2D& a, const Point2D& b);
	float distanceFrom(const Point2D& p) const;
	float squaredDistanceFrom(const Point2D& p) const;
	bool isPoint() const { return _isPoint; }
	float a() const { return _a; }
	float b() const { return _b; }	
	float c() const { return _c; }
private:

	Point2D _pass;
	bool _isPoint = false;
	// ax + by + c = 0
	float _a;
	float _b;
	float _c;
};