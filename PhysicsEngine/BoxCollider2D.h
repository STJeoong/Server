#pragma once
#include "Collider2D.h"
#include "Vector2D.h"
#include "S_BoxDef.h"
class BoxCollider2D : public Collider2D
{
	friend class GameObject;
public:
	virtual AABB getAABB() override;
	const Vector2D& halfSize() const { return _halfSize; }
	Vector2D& halfSize() { return _halfSize; }
private:
	// if you want to make BoxCollider's value default, use default constructor
	// default halfSize is (0.5f, 0.5f)
	BoxCollider2D() : _halfSize(0.5f, 0.5f) { this->compute(); }
	BoxCollider2D(const S_BoxDef& def);
	~BoxCollider2D() = default;
	virtual BoxCollider2D* clone() override;
	void onAddComponent(Component* component) override; // this is used when rigidbody is made later than collider
	void compute(); // compute mass and inerita. call this when definition of collider is changed.


	Vector2D _halfSize; // 회전이 적용되지 않은 값
};