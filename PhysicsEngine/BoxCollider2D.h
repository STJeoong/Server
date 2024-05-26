#pragma once
#include <vector>
#include "Collider2D.h"
#include "Vector2D.h"
#include "S_BoxDef.h"
class BoxCollider2D : public Collider2D
{
	friend class GameObject;
	friend class ContactGenerator;
	friend class Polytope;
	friend class Contact2D;
public:
	const Vector2D& halfSize() const { return _halfSize; }
	// TODO : halfSize 바뀌면 mass도 바뀜.
	void halfSize(const Vector2D& hs) { _halfSize = hs; }
private:
	// if you want to make BoxCollider's value default, use default constructor
	// default halfSize is (0.5f, 0.5f)
	BoxCollider2D() : _halfSize(0.5f, 0.5f) { this->compute(); }
	BoxCollider2D(const S_BoxDef& def);
	~BoxCollider2D() = default;
	virtual BoxCollider2D* clone() override;
	virtual AABB computeAABB(const Transform& transform) override;
	virtual Point2D computeSupportPoint(const Vector2D& vec) const override;
	virtual Point2D computeMarginSupportPoint(const Vector2D& vec) const override;
	virtual bool perpendicularToTheSide(const Vector2D& vec) const override;
	virtual bool project(const Vector2D& oldNormal, bool isA, const Point2D& otherGlobalPoint, const Point2D& myGlobalPoint, Point2D localEdgePoints[2]) override;
	virtual bool containsPoint(const Point2D& p) override;
	virtual void updatePoints() override;
	void compute(); // compute mass and inerita. call this when definition of collider is changed.
	void computePoints(Point2D points[4], const Transform& transform) const;
	void computeMarginPoints(Point2D points[4], const Transform& transform) const;


	Vector2D _halfSize; // 회전이 적용되지 않은 값
	Point2D _points[4];
	Point2D _marginPoints[4];
};