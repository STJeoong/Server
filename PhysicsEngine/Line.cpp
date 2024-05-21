#include "pch.h"	
#include "Line.h"

#pragma region public
Line::Line(const Point2D& a, const Point2D& b) : _pass(a)
{
	if (a == b) _isPoint = true;
	_a = -(b.y() - a.y());
	_b = b.x() - a.x();
	_c = (b.y() - a.y()) * a.x() - (b.x() - a.x()) * a.y();
}
float Line::distanceFrom(const Point2D& p) const
{
	if (_isPoint) return (p - _pass).len();
	return std::abs(_a * p.x() + _b * p.y() + _c) / std::sqrtf(_a * _a + _b * _b);
}
float Line::squaredDistanceFrom(const Point2D& p) const
{
	if (_isPoint) return (p - _pass).squaredLen();
	return ((_a * p.x() + _b * p.y() + _c) * (_a * p.x() + _b * p.y() + _c)) / (_a * _a + _b * _b);
}
bool Line::isPointOnTheLine(const Point2D& p) const
{
	return std::abs(_a * p.x() + _b * p.y() + _c) < FLT_EPSILON;
}
Point2D Line::project(const Point2D& p) const
{
	float k = -(_a * p.x() + _b * p.y() + _c) / (_a * _a + _b * _b);

	return { p.x() + _a * k, p.y() + _b * k };
}
#pragma endregion

#pragma region private
#pragma endregion
