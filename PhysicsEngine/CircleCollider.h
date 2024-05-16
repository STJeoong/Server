#pragma once
#include "Collider2D.h"
#include "Vector2D.h"
#include "S_CircleDef.h"
class CircleCollider : public Collider2D
{
	friend class Polytope;
	friend class GameObject;
public:
	float radius() const { return _radius; }
	// TODO : radius ¹Ù²î¸é massÁ¤º¸µµ ¹Ù²ñ
	void radius(float r) { _radius = r; }
private:
	CircleCollider() { _size = _radius; this->compute(); }
	CircleCollider(const S_CircleDef& def);
	~CircleCollider() = default;
	virtual CircleCollider* clone() override;
	virtual AABB computeAABB() override;
	virtual Point2D computeSupportPoint(const Vector2D& vec) override;
	void compute(); // compute mass and inerita. call this when definition of collider is changed.


	float _radius = 1.0f;
};