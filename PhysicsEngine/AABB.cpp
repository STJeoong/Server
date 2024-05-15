#include "pch.h"
#include "AABB.h"

#pragma region public
AABB::AABB(const Point2D& mini, const Point2D& maxi) { _min = mini; _max = maxi; }
float AABB::perimeter() const { Vector2D size = _max - _min; return 2 * (size.x() + size.y()); }
bool AABB::contains(const AABB& obj) const
{
	return _min.x() <= obj.mini().x() && _min.y() <= obj.mini().y() &&
		_max.x() >= obj.maxi().x() && _max.y() >= obj.maxi().y();
}
bool AABB::overlaps(const AABB& obj) const
{
	if (_max.x() < obj.mini().x() || _min.x() > obj.maxi().x()) return false;
	if (_max.y() < obj.mini().y() || _min.y() > obj.maxi().y()) return false;
	return true;
}
AABB AABB::operator+(const AABB& obj) const
{
	Point2D mini{ std::min(_min.x(), obj._min.x()), std::min(_min.y(), obj._min.y()) };
	Point2D maxi{ std::max(_max.x(), obj._max.x()), std::max(_max.y(), obj._max.y()) };
	return { mini, maxi };
}
AABB& AABB::operator+=(const Vector2D& v)
{
	if (v.x() >= 0.0f) _max.x(_max.x() + v.x());
	else _min.x(_min.x() + v.x());
	if (v.y() >= 0.0f) _max.y(_max.y() + v.y());
	else _min.y(_min.y() + v.y());
	return *this;
}
AABB AABB::operator+(const Vector2D& v) const { AABB ret = *this; ret += v; return ret; }
AABB& AABB::operator-=(const Vector2D& v) { *this += v * -1.0f; return *this; }
AABB AABB::operator-(const Vector2D& v) const { AABB ret = *this; ret -= v; return ret; }
const Point2D& AABB::mini() const { return _min; }
const Point2D& AABB::maxi() const { return _max; }
void AABB::mini(const Point2D& p) { _min = p; }
void AABB::maxi(const Point2D& p) { _max = p; }
#pragma endregion

#pragma region private
#pragma endregion