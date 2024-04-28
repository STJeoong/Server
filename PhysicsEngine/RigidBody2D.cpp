#include "pch.h"
#include "GameObject.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "World.h"

#pragma region public
RigidBody2D* RigidBody2D::clone()
{
	RigidBody2D* ret = new RigidBody2D(*this);
	ret->_colliders.resize(0);
	return ret;
}
void RigidBody2D::type(E_BodyType bodyType)
{
	if (_type == bodyType) return;
	// TODO : type 변경되면 충돌 판정 명단에서도 변경이 있어야됨.
}
void RigidBody2D::wakeUp()
{
	if (_type == E_BodyType::STATIC) return;
	_flags |= (int)E_BodyFlag::AWAKE;
	_sleepTime = 0.0f;
}
void RigidBody2D::sleep()
{
	if (_type == E_BodyType::STATIC) return;
	_flags &= ~(int)E_BodyFlag::AWAKE;
	_sleepTime = 0.0f;
	_velocity = {};
	_angularVelocity = 0.0f;
	_force = {};
	_torque = 0.0f;
}
void RigidBody2D::fixedRotation(bool flag)
{
	bool old = (_flags & (int)E_BodyFlag::FIXED_ROTATION);
	if (flag == old) return;
	if (flag) _flags |= (int)E_BodyFlag::FIXED_ROTATION;
	else _flags &= ~(int)E_BodyFlag::FIXED_ROTATION;
	_angularVelocity = 0.0f;
	this->resetMassData();
}
#pragma endregion

#pragma region private
RigidBody2D::RigidBody2D(const S_RigidDef& def)
{
	_type = def.type;
	_gravityScale = def.gravityScale;
	if (def.awake) _flags |= (int)E_BodyFlag::AWAKE;
	if (def.fixedRotation) _flags |= (int)E_BodyFlag::FIXED_ROTATION;
	if (def.autoSleep) _flags |= (int)E_BodyFlag::AUTO_SLEEP;
}
void RigidBody2D::onAddComponent(Component* component)
{
	if (typeid(*component) == typeid(RigidBody2D))
	{
		const std::vector<Component*>& components = this->gameObject()->components();
		Collider2D* tmp = nullptr;
		for (int i = 0; i < components.size(); ++i)
		{
			tmp = dynamic_cast<Collider2D*>(components[i]);
			if (tmp == nullptr) continue;
			_colliders.push_back(tmp);
		}
		this->resetMassData();
		World::addRigid(this);
		return;
	}
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	_colliders.push_back(collider);
	this->resetMassData();
}
void RigidBody2D::onRemoveComponent(Component* component)
{
	if (typeid(*component) == typeid(RigidBody2D))
	{
		for (int i = 0; i < _colliders.size(); ++i)
			_colliders[i]->attachTo(nullptr);
		World::removeRigid(this);
		return;
	}
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	auto it = std::find(_colliders.begin(), _colliders.end(), component);
	_colliders.erase(it);
	this->resetMassData();
}
void RigidBody2D::onEnableComponent(Component* component)
{
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	_colliders.push_back(collider);
	this->resetMassData();
}
void RigidBody2D::onDisableComponent(Component* component)
{
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	auto it = std::find(_colliders.begin(), _colliders.end(), component);
	_colliders.erase(it);
	this->resetMassData();
}
void RigidBody2D::resetMassData()
{
	_mass = 0.0f;
	_invMass = 0.0f;
	_inertia = 0.0f;
	_invInertia = 0.0f;

	if (_type == E_BodyType::STATIC || _type == E_BodyType::KINEMATIC) return;
	_localCOM = { 0.0f,0.0f };
	for (int i = 0; i < _colliders.size(); ++i)
	{
		Collider2D& collider = *_colliders[i];
		if (collider.density() == 0.0f || collider.isTrigger()) continue;
		_mass += collider.mass();
		_localCOM += collider.offset() * collider.mass(); // TODO : polygon은 offset * mass 하면 안됨.
		_inertia += collider.inertia();
	}
	if (_mass == 0.0f) _mass = 1.0f; // 질량 역할을 하는 콜라이더가 하나도 없으면
	_invMass = 1.0f / _mass;
	_localCOM *= _invMass;

	if (_inertia > 0.0f && !this->fixedRotation())
	{
		_inertia -= _localCOM.squaredLen() * _mass;
		_invInertia = 1.0f / _inertia;
	}
	else
		_inertia = 0.0f;


	// TODO
	//// Move center of mass.
	//b2Vec2 oldCenter = m_sweep.c;
	//m_sweep.localCenter = localCenter;
	//m_sweep.c0 = m_sweep.c = b2Mul(m_xf, m_sweep.localCenter);

	//// Update center of mass velocity.
	//m_linearVelocity += b2Cross(m_angularVelocity, m_sweep.c - oldCenter);
}
#pragma endregion

