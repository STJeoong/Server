#pragma once
#include "Behaviour.h"
#include "Vector2D.h"
#include "Utils.h"

class RigidBody2D;
class AABB;
class Collider2D : public Behaviour
{
	friend class RigidBody2D;
public:
	virtual AABB getAABB() = 0;
	virtual void enabled(bool flag) override;
	const Point2D& offset() const { return _offset; } // local offset
	const float& density() const { return _density; }
	const float& friction() const { return _friction; }
	const float& bounciness() const { return _bounciness; }
	const float& mass() const { return _mass; }
	const float& inertia() const { return _inertia; }
	const bool& isTrigger() const { return _isTrigger; }
	RigidBody2D* attachedRigidBody() const { return _attachedRigidBody; }
	void friction(float f) { _friction = Utils::clamp(f, 100000.0f, 0.0f); }
	void bounciness(float b) { _bounciness = Utils::clamp(b, 100000.0f, 0.0f); }
	// TODO : offset, density, trigger 바뀌면 rigidbody에 이벤트 보내도록
protected:
	Collider2D() = default;
	virtual ~Collider2D() = default;
	virtual void onAddComponent(Component* component) override; // this is used when rigidbody is made later than collider
	virtual void onRemoveComponent(Component* component) override; // this is used when rigidbody is made later than collider

	Point2D _offset;
	float _density = 1.0f;
	float _friction = 0.4f;
	float _bounciness = 0.0f;
	float _mass = 0.0f; // you need to keep this value up to date. update this whenever related to mass is changed.
	float _inertia = 0.0f; // you need to keep this value up to date. update this whenever related to inertia is changed.
	bool _isTrigger = false;
	RigidBody2D* _attachedRigidBody = nullptr;
private:
	void addToBroadPhase();
	void removeFromBroadPhase();
	void attachTo(RigidBody2D* rigid) { _attachedRigidBody = rigid; }
	const int& key() const { return _key; }

	int _key = -1; // DAT node index
};