#include "pch.h"
#include "Solver.h"
#include "Collision2D.h"
#include "Vector2D.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "Contact2D.h"
#include "GameObject.h"

#include "AABB.h"
#pragma region public
void Solver::init(const std::vector<Collision2D*>& collisions)
{
	this->warmStart(collisions);
	_velocityBiases.resize(collisions.size());
	_normalEffMasses.resize(collisions.size());
	_tangentEffMasses.resize(collisions.size());
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();

		_velocityBiases[i].resize(contacts.size());
		_normalEffMasses[i].resize(contacts.size());
		_tangentEffMasses[i].resize(contacts.size());
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			this->computeBouncinessBias(contact, collision._bounciness, collision._bouncinessThreshold);
			_velocityBiases[i][j] = contact._bouncinessBias;
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
			float oldTangentImpulse = contact._tangentImpulse;
			float maxTangentImpulse = contact._normalImpulse * collision._friction;
			contact._tangentImpulse = Utils::clamp(oldTangentImpulse + tLambda, maxTangentImpulse, -maxTangentImpulse);
			tLambda = contact._tangentImpulse - oldTangentImpulse;
			this->impulse(contact, contact.tangent(), tLambda);

			float nLambda = -(this->computeJV(contact, contact.normal()) + _velocityBiases[i][j]) * _normalEffMasses[i][j];
			if (nLambda < 0.0f)
				nLambda = -std::min(std::abs(nLambda), contact._normalImpulse);
			contact._normalImpulse += nLambda;
			this->impulse(contact, contact.normal(), nLambda);
		}
	}
}
void Solver::solvePositionConstraints(const std::vector<Collision2D*>& collisions)
{
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);

			RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
			RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
			Collider2D* colliderA = contact.colliderA();
			Collider2D* colliderB = contact.colliderB();
			Point2D globalPointA = colliderA->toWorld(contact.localContactA());
			Point2D globalPointB = colliderB->toWorld(contact.localContactB());
			float depth = Vector2D::dot(globalPointA - globalPointB, contact.normal());
			float bias = this->computePenetrationBias(contact, depth);
			if (bias < 0.0f)
				contact._isUsed = true;
			float nLambda = -bias * _normalEffMasses[i][j];
			
			GameObject* objA = colliderA->gameObject();
			GameObject* objB = colliderB->gameObject();
			float invMassA = rigidA == nullptr ? 0.0f : rigidA->invMass();
			float invMassB = rigidB == nullptr ? 0.0f : rigidB->invMass();
			float invInertiaA = rigidA == nullptr ? 0.0f : rigidA->invInertia();
			float invInertiaB = rigidB == nullptr ? 0.0f : rigidB->invInertia();
			Vector2D p = nLambda * contact.normal();
			{
				Vector2D dv = -invMassA * p;
				float dw = -invInertiaA * Vector2D::cross(contact._rA, p);
				Motion motion = { dv, dw };
				objA->processTransform(motion);
			}
			{
				Vector2D dv = invMassB * p;
				float dw = invInertiaB * Vector2D::cross(contact._rB, p);
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
		rigidA->_angularVelocity -= invInertiaA * Vector2D::cross(contact._rA, p);
	}
	if (rigidB != nullptr)
	{
		rigidB->_velocity += invMassB * p;
		rigidB->_angularVelocity += invInertiaB * Vector2D::cross(contact._rB, p);
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
				rigidA->_angularVelocity -= Vector2D::cross(contact._rA, v) * rigidA->invInertia();
			}
			if (rigidB != nullptr)
			{
				rigidB->_velocity += v * rigidB->invMass();
				rigidB->_angularVelocity += Vector2D::cross(contact._rB, v) * rigidB->invInertia();
			}
		}
	}
}
float Solver::computePenetrationBias(const Contact2D& contact, float depth)
{
	Collider2D* colliderA = contact.colliderA();
	Collider2D* colliderB = contact.colliderB();
	return -std::max(BAUMGART * (depth - Solver::PENETRATION_SLOP), 0.0f);
	//return -Utils::clamp(BAUMGART * (depth - slop), maxCorrection, 0.0f);
	//return -Utils::clamp(BAUMGART * (depth - slop), Solver::MAX_LINEAR_CORRECTION, 0.0f);
}
void Solver::computeBouncinessBias(Contact2D& contact, float bounciness, float bouncinessThreshold)
{
	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();

	Vector2D relativeVel = {};
	float normalVel = 0.0f;

	if (rigidA != nullptr && rigidA->type() != E_BodyType::STATIC)
		relativeVel -= rigidA->velocity() + Vector2D::cross(rigidA->angularVelocity(), contact._rA);
	if (rigidB != nullptr && rigidB->type() != E_BodyType::STATIC)
		relativeVel += rigidB->velocity() + Vector2D::cross(rigidB->angularVelocity(), contact._rB);
	normalVel = Vector2D::dot(relativeVel, contact.normal());

	if (-normalVel > bouncinessThreshold)
		contact._bouncinessBias = bounciness * normalVel;
}
float Solver::computeEffectiveMass(const Contact2D& contact, const Vector2D& dir)
{
	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
	float invMassA = rigidA == nullptr ? 0.0f : rigidA->invMass();
	float invMassB = rigidB == nullptr ? 0.0f : rigidB->invMass();
	float invInertiaA = rigidA == nullptr ? 0.0f : rigidA->invInertia();
	float invInertiaB = rigidB == nullptr ? 0.0f : rigidB->invInertia();

	float rdA = Vector2D::cross(contact._rA, dir);
	float rdB = Vector2D::cross(contact._rB, dir);
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

const float Solver::PENETRATION_SLOP = 0.03f;