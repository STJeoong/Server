#pragma once
#include <vector>
#include "Collision2D.h"
#include "Behaviour.h"
#include "Vector2D.h"
#include "Utils.h"

class RigidBody2D;
class AABB;
class Transform;
class Collider2D : public Behaviour
{
	friend class RigidBody2D;
	friend class CollisionDetector;
	friend class Polytope;
	friend class ContactGenerator;
	friend class World;
	friend class Contact2D;
public:
	const Point2D& offset() const { return _offset; } // local offset
	float density() const { return _density; }
	float friction() const { return _friction; }
	float bounciness() const { return _bounciness; }
	float bouncinessThreshold() const { return _bouncinessThreshold; }
	float mass() const { return _mass; }
	float inertia() const { return _inertia; }
	bool isTrigger() const { return _isTrigger; }
	void isTrigger(bool flag);
	RigidBody2D* attachedRigidBody() const { return _attachedRigidBody; }
	void friction(float f) { _friction = Utils::clamp(f, 100000.0f, 0.0f); }
	void bounciness(float b) { _bounciness = Utils::clamp(b, 100000.0f, 0.0f); }
	const std::vector<Collision2D*>& collisions() const { return _collisions; }
	// get world position of collider's center
	Point2D position() const;
	// compute local position from given world point
	Point2D toLocal(const Point2D& p) const;
	// compute world position from given local point
	Point2D toWorld(const Point2D& p) const;
	// TODO : offset, density, trigger 바뀌면 rigidbody에 이벤트 보내도록
	// TODO : collider size가 바뀔 수도 있음.
protected:
	Collider2D() = default;
	virtual ~Collider2D() = default;
	virtual void onDestroy() override;
	virtual void onActiveGameObject() override;
	virtual void onInactiveGameObject() override;
	virtual bool onAddComponent(Component* component) override;
	virtual void onRemoveComponent(Component* component) override;
	virtual void onEnableComponent(Component* component) override;
	virtual void onDisableComponent(Component* component) override;
	virtual void onApplyReservation() override;
	virtual void onMove() override;

	Point2D _offset;
	float _density = 1.0f;
	float _friction = 0.4f;
	float _bounciness = 0.0f;
	float _bouncinessThreshold = 1.0f;
	float _mass = 0.0f; // you need to keep this value up to date. update this whenever related to mass is changed.
	float _inertia = 0.0f; // you need to keep this value up to date. update this whenever related to inertia is changed.
	bool _isTrigger = false;
	RigidBody2D* _attachedRigidBody = nullptr;
	std::vector<Collision2D*> _collisions;

	// reservation ( executed next time step )
	bool _needToToggleTriggerState = false; // have you reserved modification of trigger state?
private:
	virtual AABB computeAABB(const Transform& transform) = 0;
	virtual Point2D computeSupportPoint(const Vector2D& vec) const = 0;
	virtual Point2D computeMarginSupportPoint(const Vector2D& vec) const = 0;
	virtual bool isCircle() const { return false; }
	virtual bool perpendicularToTheSide(const Vector2D& vec) const = 0;
	virtual bool project(const Vector2D& oldNormal, bool isA, const Point2D& otherGlobalPoint, const Point2D& myGlobalPoint, Point2D localEdgePoints[2]) = 0;
	virtual void updatePoints() = 0;
	void addToWorld();
	void removeFromWorld();
	void attachTo(RigidBody2D* rigid) { _attachedRigidBody = rigid; }
	const int& key() const { return _key; }

	int _key = -1; // DAT node index
};