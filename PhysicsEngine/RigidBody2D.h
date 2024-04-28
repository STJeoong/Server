#pragma once
#include <vector>
#include "Vector2D.h"
#include "Component.h"
#include "E_BodyType.h"
#include "E_BodyFlag.h"
#include "S_RigidDef.h"

class Collider2D;
class RigidBody2D : public Component
{
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
	bool isAwake() { return _flags & (int)E_BodyFlag::AWAKE; }
	void wakeUp();
	void sleep();
private:
	RigidBody2D() = default;
	RigidBody2D(const S_RigidDef& def);
	~RigidBody2D() = default;
	virtual RigidBody2D* clone() override;
	void onAddComponent(Component* component) override;
	void onRemoveComponent(Component* component) override;
	void onEnableComponent(Component* component) override;
	void onDisableComponent(Component* component) override;
	void resetMassData();

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
	UINT8 _flags = 1; // default : awake start
	float _sleepTime = 0.0f;

	std::vector<Collider2D*> _colliders;
};