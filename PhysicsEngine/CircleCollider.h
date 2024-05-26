#pragma once
#include "Collider2D.h"
#include "Vector2D.h"
#include "S_CircleDef.h"
class CircleCollider : public Collider2D
{
	friend class GameObject;
	friend class ContactGenerator;
	friend class Polytope;
	friend class Contact2D;
public:
	float radius() const { return _radius; }
	// TODO : radius ¹Ù²î¸é massÁ¤º¸µµ ¹Ù²ñ
	void radius(float r) { _radius = r; }
private:
	CircleCollider() { this->compute(); }
	CircleCollider(const S_CircleDef& def);
	~CircleCollider() = default;
	virtual CircleCollider* clone() override;
	virtual AABB computeAABB(const Transform& transform) override;
	virtual Point2D computeSupportPoint(const Vector2D& vec) const override;
	virtual Point2D computeMarginSupportPoint(const Vector2D& vec) const override;
	virtual bool perpendicularToTheSide(const Vector2D& vec) const override;
	virtual bool project(const Vector2D& oldNormal, bool isA, const Vector2D& otherGlobalPoint, const Point2D& myGlobalPoint, Point2D localEdgePoints[2]) override;
	virtual bool containsPoint(const Point2D& p) override;
	virtual void updatePoints() override {}
	virtual bool isCircle() const { return true; }
	void compute(); // compute mass and inerita. call this when definition of collider is changed.


	float _radius = 1.0f;
};