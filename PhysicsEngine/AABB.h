#pragma once
#include "Vector2D.h"

class AABB
{
public:
	AABB() {}
	AABB(const Point2D& mini, const Point2D& maxi);
	float perimeter() const; 
	bool contains(const AABB& obj) const;
	bool overlaps(const AABB& obj) const;
	AABB operator+(const AABB& obj) const;
	AABB& operator+=(const Vector2D& v);
	AABB operator+(const Vector2D& v) const; 
	AABB& operator-=(const Vector2D& v);
	AABB operator-(const Vector2D& v) const;
	const Point2D& mini() const;
	const Point2D& maxi() const;
	void mini(const Point2D& p);
	void maxi(const Point2D& p);
private:
	Point2D _min;
	Point2D _max;
};