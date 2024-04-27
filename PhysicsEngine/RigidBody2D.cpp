#include "pch.h"
#include "GameObject.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "World.h"

#pragma region public
RigidBody2D::RigidBody2D() { World::addRigid(this); }
RigidBody2D::~RigidBody2D() { World::removeRigid(this); }
RigidBody2D* RigidBody2D::clone()
{
	RigidBody2D* ret = new RigidBody2D(*this);
	ret->_colliders.resize(0);
	return ret;
}
void RigidBody2D::type(E_BodyType bodyType)
{
	if (_type == bodyType) return;
	// TODO : type ����Ǹ� �浹 ���� ��ܿ����� ������ �־�ߵ�.
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
void RigidBody2D::onAddComponent(Component* component)
{
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	this->addCollider(collider);
}
void RigidBody2D::onRemoveComponent(Component* component)
{
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	this->removeCollider(collider);
}
void RigidBody2D::onActiveGameObject()
{
	// TODO : ������ �ִ� ��� �ݶ��̴� ���忡 push
}
void RigidBody2D::onInactiveGameObject()
{
	// TODO : world�� �ִ� ���õ� �ݶ��̴� ����
}
void RigidBody2D::onEnableComponent(Component* component)
{
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	this->addCollider(collider);
}
void RigidBody2D::onDisableComponent(Component* component)
{
	Collider2D* collider = dynamic_cast<Collider2D*>(component);
	if (collider == nullptr) return;
	this->removeCollider(collider);
}
void RigidBody2D::addCollider(Collider2D* collider)
{
	_colliders.push_back(collider);
	this->resetMassData();
	if (!this->gameObject()->isActive()) return;
	// TODO : world�� broadphase�� �߰�
}
void RigidBody2D::removeCollider(Collider2D* collider)
{
	for (int i = 0; i < _colliders.size(); ++i)
		if (_colliders[i] == collider)
		{
			_colliders.erase(_colliders.begin() + i);
			break;
		}
	this->resetMassData();
	if (!this->gameObject()->isActive()) return;
	// TODO : world�� broadphase���� ����
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
		_localCOM += collider.offset() * collider.mass(); // TODO : polygon�� offset * mass �ϸ� �ȵ�.
		_inertia += collider.inertia();
	}
	if (_mass == 0.0f) _mass = 1.0f; // ���� ������ �ϴ� �ݶ��̴��� �ϳ��� ������
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

