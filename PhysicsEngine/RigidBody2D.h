#pragma once
#include <vector>
#include "Vector2D.h"
#include "Component.h"
#include "E_BodyType.h"
#include "E_BodyFlag.h"
#include "S_RigidDef.h"
#include "Transform.h"

class Collider2D;
class RigidBody2D : public Component
{
	friend class GameObject;
	friend class Solver;
	friend class World;
public:
	void addForce(const Vector2D& force) { if (_type != E_BodyType::DYNAMIC) return; _force += force; }
	void addTorque(float torque) { if (_type != E_BodyType::DYNAMIC) return; _torque += torque; }
	const Vector2D& velocity() const { return _velocity; }
	float angularVelocity() const { return _angularVelocity; }
	void velocity(const Vector2D& v) { if (_type == E_BodyType::STATIC) return; _velocity = v; }
	void angularVelocity(float w) { if (_type == E_BodyType::STATIC) return; _angularVelocity = w; }
	float gravityScale() const { return _gravityScale; }
	void gravityScale(float scale) { _gravityScale = scale; }
	float mass() const { return _mass; }
	float invMass() const { return _invMass; }
	float inerita() const { return _inertia; }
	float invInertia() const { return _invInertia; }
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
	~RigidBody2D();
	virtual RigidBody2D* clone() override;
	void onUpdate() override;
	bool onAddComponent(Component* component) override;
	void onRemoveComponent(Component* component) override;
	void onEnableComponent(Component* component) override;
	void onDisableComponent(Component* component) override;
	void onApplyReservation() override;
	void sync();
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

	E_BodyType _type = E_BodyType::DYNAMIC;
	float _gravityScale = 1.0f;
	UINT8 _flags = 1; // default : awake start
	float _sleepTime = 0.0f;
	Transform _oldTransform;

	std::vector<Collider2D*> _colliders;

	int _key = -1; // world key

	// reservation ( executed next time step )
	E_BodyType _newType = E_BodyType::DYNAMIC;
	bool _wasAdded = false;
};