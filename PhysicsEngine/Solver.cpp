#include "pch.h"
#include "Solver.h"
#include "Collision2D.h"
#include "Vector2D.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "Contact2D.h"
#include "GameObject.h"

#include <ThreadPool.h>
#pragma region public
void Solver::init(const std::vector<Collision2D*>& collisions)
{
	this->warmStart(collisions);
	_normalEffMasses.resize(collisions.size());
	_tangentEffMasses.resize(collisions.size());
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();

		collision.computeBouncinessBias();
		_normalEffMasses[i].resize(contacts.size());
		_tangentEffMasses[i].resize(contacts.size());
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			_normalEffMasses[i][j] = this->computeEffectiveMass(contact, contact.normal());
			_tangentEffMasses[i][j] = this->computeEffectiveMass(contact, contact.tangent());
		}
	}
}
void Solver::integrateVelocity(const std::vector<RigidBody2D*>& rigids, const Vector2D& g, float dt)
{
	for (RigidBody2D* rigid : rigids)
		if (rigid->type() == E_BodyType::DYNAMIC)
		{
			float gScale = rigid->_gravityScale;
			float invMass = rigid->invMass();
			float invI = rigid->invInertia();
			const Vector2D& force = rigid->_force;
			float torque = rigid->_torque;
			rigid->_velocity += {(force.x()* invMass + g.x() * gScale)* dt, (force.y()* invMass + g.y() * gScale)* dt};
			rigid->_angularVelocity += torque * invI;

			// clear force and torque
			rigid->_force = {};
			rigid->_torque = 0;
		}
}
void Solver::integratePosition(const std::vector<RigidBody2D*>& rigids, float dt)
{
	for (RigidBody2D* rigid : rigids)
	{
		GameObject* obj = rigid->gameObject();
		Motion motion = { rigid->velocity() * dt, rigid->angularVelocity() * dt };
		obj->processTransform(motion);
	}
}
void Solver::solveVelocityConstraints(const std::vector<Collision2D*>& collisions)
{
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			float tLambda = -this->computeJV(contact, contact.tangent()) * _tangentEffMasses[i][j];
			float oldTangentImpulse = contact.tangentImpulse();
			float maxTangentImpulse = contact.normalImpulse() * collision._friction;
			contact._tangentImpulse = Utils::clamp(oldTangentImpulse + tLambda, maxTangentImpulse, -maxTangentImpulse);
			tLambda = contact.tangentImpulse() - oldTangentImpulse;
			this->impulse(contact, contact.tangent(), tLambda);

			float nLambda = -(this->computeJV(contact, contact.normal()) + contact.bouncinessBias()) * _normalEffMasses[i][j];
			if (nLambda < 0.0f)
				nLambda = -std::min(std::abs(nLambda), contact.normalImpulse());
			contact._normalImpulse += nLambda;
			this->impulse(contact, contact.normal(), nLambda);
		}
	}
}
void Solver::test(const std::vector<Collision2D*>& collisions, size_t start, size_t end)
{
	if (end - start < 100)
	{
		_lastKey = ThreadPool::enqueue([&collisions, start, end]() {
			for (int i = start; i < end; ++i)
				collisions[i]->updateContacts();
			});
		return;
	}
	size_t mid = (start + end) / 2;
	this->test(collisions, start, mid);
	this->test(collisions, mid, end);
}
void Solver::solvePositionConstraints(const std::vector<Collision2D*>& collisions)
{
	this->test(collisions, 0, collisions.size());
	ThreadPool::join(_lastKey);
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();
		//collision.updateContacts();
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);

			RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
			RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
			Collider2D* colliderA = contact.colliderA();
			Collider2D* colliderB = contact.colliderB();
			float invMassA = rigidA == nullptr ? 0.0f : rigidA->invMass();
			float invMassB = rigidB == nullptr ? 0.0f : rigidB->invMass();
			float invInertiaA = rigidA == nullptr ? 0.0f : rigidA->invInertia();
			float invInertiaB = rigidB == nullptr ? 0.0f : rigidB->invInertia();
			float bias = this->computePenetrationBias(contact.depth());

			float rnA = Vector2D::cross(contact.rA(), contact.normal());
			float rnB = Vector2D::cross(contact.rB(), contact.normal());
			float val = invMassA + invMassB + invInertiaA * rnA * rnA + invInertiaB * rnB * rnB;
			float effMass = 0.0f;
			if (val > 0.0f)
				effMass = 1 / val;
			float nLambda = -bias * effMass;
			
			GameObject* objA = colliderA->gameObject();
			GameObject* objB = colliderB->gameObject();
			Vector2D p = nLambda * contact.normal();
			{
				Vector2D dv = -invMassA * p;
				float dw = -invInertiaA * Vector2D::cross(contact.rA(), p);
				Motion motion = { dv, dw };
				objA->processTransform(motion);
			}
			{
				Vector2D dv = invMassB * p;
				float dw = invInertiaB * Vector2D::cross(contact.rB(), p);
				Motion motion = { dv, dw };
				objB->processTransform(motion);
			}
		}
	}
}
#pragma endregion

#pragma region private
void Solver::impulse(const Contact2D& contact, const Vector2D& dir, float lambda)
{
	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
	float invMassA = rigidA == nullptr ? 0.0f : rigidA->invMass();
	float invMassB = rigidB == nullptr ? 0.0f : rigidB->invMass();
	float invInertiaA = rigidA == nullptr ? 0.0f : rigidA->invInertia();
	float invInertiaB = rigidB == nullptr ? 0.0f : rigidB->invInertia();

	Vector2D p = dir * lambda;
	if (rigidA != nullptr)
	{
		rigidA->_velocity -= invMassA * p;
		rigidA->_angularVelocity -= invInertiaA * Vector2D::cross(contact.rA(), p);
	}
	if (rigidB != nullptr)
	{
		rigidB->_velocity += invMassB * p;
		rigidB->_angularVelocity += invInertiaB * Vector2D::cross(contact.rB(), p);
	}
}
void Solver::warmStart(const std::vector<Collision2D*>& collisions)
{
	for (int i = 0; i < collisions.size(); ++i)
	{
		const Collision2D& collision = *(collisions[i]);
		const std::vector<Contact2D*>& contacts = collision.contacts();
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			Vector2D v = contact.normal() * contact.normalImpulse() + contact.tangent() * contact.tangentImpulse();

			RigidBody2D* rigidA = collision.colliderA()->attachedRigidBody();
			RigidBody2D* rigidB = collision.colliderB()->attachedRigidBody();
			if (rigidA != nullptr)
			{
				rigidA->_velocity -= v * rigidA->invMass();
				rigidA->_angularVelocity -= Vector2D::cross(contact.rA(), v) * rigidA->invInertia();
			}
			if (rigidB != nullptr)
			{
				rigidB->_velocity += v * rigidB->invMass();
				rigidB->_angularVelocity += Vector2D::cross(contact.rB(), v) * rigidB->invInertia();
			}
		}
	}
}
float Solver::computePenetrationBias(float depth)
{
	return -std::max(BAUMGART * (depth - Solver::PENETRATION_SLOP), 0.0f);
	float bias = -std::max(BAUMGART * (depth - Solver::PENETRATION_SLOP), 0.0f);
	bias = std::max(bias, -Solver::MAX_LINEAR_CORRECTION);
	return bias;
}
float Solver::computeEffectiveMass(const Contact2D& contact, const Vector2D& dir)
{
	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
	float invMassA = rigidA == nullptr ? 0.0f : rigidA->invMass();
	float invMassB = rigidB == nullptr ? 0.0f : rigidB->invMass();
	float invInertiaA = rigidA == nullptr ? 0.0f : rigidA->invInertia();
	float invInertiaB = rigidB == nullptr ? 0.0f : rigidB->invInertia();

	float rdA = Vector2D::cross(contact.rA(), dir);
	float rdB = Vector2D::cross(contact.rB(), dir);
	float ret = invMassA + invMassB + invInertiaA * rdA * rdA + invInertiaB * rdB * rdB;
	return ret > 0.0f ? 1 / ret : 0.0f;
}
float Solver::computeJV(const Contact2D& contact, const Vector2D& dir)
{
	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
	Vector2D relativeVelocity;
	if (rigidA != nullptr)
	{
		relativeVelocity -= rigidA->velocity();
		relativeVelocity -= Vector2D::cross(rigidA->angularVelocity(), contact._rA);
	}
	if (rigidB != nullptr)
	{
		relativeVelocity += rigidB->velocity();
		relativeVelocity += Vector2D::cross(rigidB->angularVelocity(), contact._rB);
	}
	return Vector2D::dot(relativeVelocity, dir);
}
#pragma endregion

const float Solver::MAX_LINEAR_CORRECTION = 0.2f;
const float Solver::PENETRATION_SLOP = 0.01f;