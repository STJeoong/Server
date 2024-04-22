#pragma once
#include "Vector2D.h"

class AABB
{
public:
	AABB() {}
	AABB(const Point2D& mini, const Point2D& maxi) { _min = mini; _max = maxi; }
	float perimeter() const { Vector2D size = _max - _min; return 2 * (size.x() + size.y()); }
	bool contains(const AABB& obj) const
	{
		return _min.x() <= obj.mini().x() && _min.y() <= obj.mini().y() &&
			_max.x() >= obj.maxi().x() && _max.y() >= obj.maxi().y();
	}
	AABB operator+(const AABB& obj) const
	{
		Point2D mini{ min(_min.x(), obj._min.x()), min(_min.y(), obj._min.y()) };
		Point2D maxi{ max(_max.x(), obj._max.x()), max(_max.y(), obj._max.y()) };
		return { mini, maxi };
	}
	AABB& operator+=(const Vector2D& v)
	{
		if (v.x() >= 0.0f) _max.x(_max.x() + v.x());
		else _min.x(_min.x() + v.x());
		if (v.y() >= 0.0f) _max.y(_max.y() + v.y());
		else _min.y(_min.y() + v.y());
		return *this;
	}
	AABB operator+(const Vector2D& v) const { AABB ret = *this; ret += v; return ret; }
	AABB& operator-=(const Vector2D& v) { *this += v * -1.0f; return *this; }
	AABB operator-(const Vector2D& v) const { AABB ret = *this; ret -= v; return ret; }
	const Point2D& mini() const { return _min; }
	const Point2D& maxi() const { return _max; }
	void mini(const Point2D& p) { _min = p; }
	void maxi(const Point2D& p) { _max = p; }
private:
	Point2D _min;
	Point2D _max;
};