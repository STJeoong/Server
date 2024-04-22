#pragma once
#include "Collider2D.h"
#include "Vector2D.h"
class Box2D : public Collider2D
{
public:
	virtual AABB getAABB() override;
	const Point2D& center() const { return _center; }
	const Vector2D& halfSize() const { return _halfSize; }
	const float& rotation() const { return _rotation; }
	void center(const Point2D& p) { _center = p; }
	void halfSize(const Vector2D& hs) { _halfSize = hs; }
	void rotation(float r) { _rotation = r; }
private:
	Point2D _center;
	Vector2D _halfSize; // 회전이 적용되지 않은 값
	float _rotation; // radian
};