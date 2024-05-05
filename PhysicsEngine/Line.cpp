#include "pch.h"	
#include "Line.h"

#pragma region public
Line::Line(const Point2D& a, const Point2D& b) : _pass(a)
{
	if (a == b) _isPoint = true;
	_a = b.x() - a.x();
	_b = -1.0f * (b.y() - a.y());
	_c = (b.y() - a.y()) * a.x() - (b.x() - a.x()) * a.y();
}
float Line::distanceFrom(const Point2D& p) const
{
	if (_isPoint) return (p - _pass).len();
	return std::abs(_a * p.x() + _b * p.y() + _c) / std::sqrtf(_a * _a + _b * _b);
}
#pragma endregion

#pragma region private
#pragma endregion
