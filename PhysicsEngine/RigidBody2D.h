#pragma once
#include <vector>
#include "Vector2D.h"
#include "Component.h"
#include "E_BodyType.h"
#include "E_BodyFlag.h"

class Collider2D;
class RigidBody2D : public Component
{
	friend class Collider2D;
	friend class GameObject;
public:
	void addForce(const Vector2D& force) { _force += force; }
	void addTorque(float torque) { _torque += torque; }
	const float& mass() const { return _mass; }
	const float& invMass() const { return _invMass; }
	const E_BodyType& type() const { return _type; }
	bool fixedRotation() const { return _flags & (int)E_BodyFlag::FIXED_ROTATION; }
	void fixedRotation(bool flag);
	void type(E_BodyType bodyType);
private:
	RigidBody2D();
	~RigidBody2D();
	virtual RigidBody2D* clone() override;
	void onAddComponent(Component* component) override; // this is used when collider is made later than rigidbody.
	void onRemoveComponent(Component* component) override; // this is used when collider is removed later than rigidbody.
	void onActiveGameObject() override;
	void onInactiveGameObject() override;
	void onEnableComponent(Component* component) override;
	void onDisableComponent(Component* component) override;
	void addCollider(Collider2D* collider);
	void removeCollider(Collider2D* collider);
	void resetMassData();

	Point2D _worldCOM; // world center of mass
	Point2D _localCOM; // local center of mass
	Vector2D _velocity;
	float _angularVelocity = 0.0f;
	Vector2D _force;
	float _torque = 0.0f;

	float _mass = 0.0f;
	float _invMass = FLT_MAX;
	float _inertia = 0.0f;
	float _invInertia = FLT_MAX;

	E_BodyType _type = E_BodyType::STATIC;
	float _gravityScale = 1.0f;
	UINT8 _flags = 0;

	std::vector<Collider2D*> _colliders; // TODO : 없애도 되나?
};