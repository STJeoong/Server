#pragma once
#include "Collider2D.h"
#include "Vector2D.h"
class Circle : public Collider2D
{
public:
	virtual AABB getAABB() override;
	const Point2D& center() const { return _center; }
	const float& radius() const { return _radius; }
	void center(const Point2D& p) { _center = p; }
	void radius(float r) { _radius = r; }
private:
	Point2D _center;
	float _radius;
};