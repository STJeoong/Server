#pragma once
#include <vector>
#include "Collider2D.h"
#include "Vector2D.h"
#include "S_BoxDef.h"
class BoxCollider2D : public Collider2D
{
	friend class GameObject;
public:
	virtual AABB computeAABB() override;
	virtual Point2D computeSupportPoint(const Vector2D& vec) override;
	const Vector2D& halfSize() const { return _halfSize; }
	// TODO : halfSize 바뀌면 mass도 바뀜.
	void halfSize(const Vector2D& hs) { _halfSize = hs; }
private:
	// if you want to make BoxCollider's value default, use default constructor
	// default halfSize is (0.5f, 0.5f)
	BoxCollider2D() : _halfSize(0.5f, 0.5f) { _size = _halfSize.len(); this->compute(); }
	BoxCollider2D(const S_BoxDef& def);
	~BoxCollider2D() = default;
	virtual BoxCollider2D* clone() override;
	void compute(); // compute mass and inerita. call this when definition of collider is changed.
	void computePoints();


	Vector2D _halfSize; // 회전이 적용되지 않은 값
	std::vector<Point2D> _points;
};