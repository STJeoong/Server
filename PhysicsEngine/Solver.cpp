#include "pch.h"
#include "Solver.h"
#include "Collision2D.h"
#include "Vector2D.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "Contact2D.h"
#include "GameObject.h"

#pragma region public
Solver::Solver(const std::vector<Collision2D*>& collisions, float dt)
{
	this->warmStart(collisions);
	_biases.resize(collisions.size());
	_normalJacobians.resize(collisions.size());
	_tangentJacobians.resize(collisions.size());
	_masses.resize(collisions.size());
	_normalEffMasses.resize(collisions.size());
	_tangentEffMasses.resize(collisions.size());
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();

		this->getMassAndInertia(collision, std::get<0>(_masses[i]), std::get<1>(_masses[i]), std::get<2>(_masses[i]), std::get<3>(_masses[i]));
		_biases[i].resize(contacts.size());
		_normalJacobians[i].resize(contacts.size());
		_tangentJacobians[i].resize(contacts.size());
		_normalEffMasses[i].resize(contacts.size());
		_tangentEffMasses[i].resize(contacts.size());
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			this->computeBouncinessBias(contact, collision._bounciness, collision._bouncinessThreshold);
			_biases[i][j] = contact._bouncinessBias + this->computePenetrationBias(contact, dt);
			_normalJacobians[i][j] = { -contact.normal().x(), -contact.normal().y(), -Vector2D::cross(contact._rA, contact.normal()),
						contact.normal().x(), contact.normal().y(), Vector2D::cross(contact._rB, contact.normal()) };
			_tangentJacobians[i][j] = { -contact.tangent().x(), -contact.tangent().y(), -Vector2D::cross(contact._rA, contact.tangent()),
						contact.tangent().x(), contact.tangent().y(), Vector2D::cross(contact._rB, contact.tangent()) };
			_normalEffMasses[i][j] = this->computeEffectiveMass(_normalJacobians[i][j], _masses[i]);
			_tangentEffMasses[i][j] = this->computeEffectiveMass(_tangentJacobians[i][j], _masses[i]);
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
			//rigid->_velocity *= 1.0f / (1.0f + dt * Solver::LINEAR_DAMPING);
			//rigid->_angularVelocity *= 1.0f / (1.0f + dt * Solver::ANGULAR_DAMPING);

			// clear force and torque
			rigid->_force = {};
			rigid->_torque = 0;
		}
}
void Solver::integratePosition(const std::vector<RigidBody2D*>& rigids, float dt)
{
	for (RigidBody2D* rigid : rigids)
		if (rigid->type() != E_BodyType::STATIC)
		{
			GameObject* obj = rigid->gameObject();
			Motion motion = { rigid->velocity() * dt, rigid->angularVelocity() * dt };
			obj->processTransform(motion);
		}
}
void Solver::resolve(const std::vector<Collision2D*>& collisions)
{
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& collision = *(collisions[i]);
		if (collision._isTrigger) continue;
		const std::vector<Contact2D*>& contacts = collision.contacts();
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			
			float nLambda = this->computeLambda(contact, _normalJacobians[i][j], _normalEffMasses[i][j], _biases[i][j]);
			if (nLambda < 0.0f)
				nLambda = std::min(std::abs(nLambda), contact._normalImpulse) * -1.0f;
			contact._normalImpulse += nLambda;
			this->impulse(contact, _normalJacobians[i][j], _masses[i], nLambda);

			float tLambda = this->computeLambda(contact, _tangentJacobians[i][j], _tangentEffMasses[i][j], 0);
			if (std::abs(contact._tangentImpulse + tLambda) > collision._friction * contact._normalImpulse)
				tLambda -= (std::abs(contact._tangentImpulse + tLambda) - (collision._friction * contact._normalImpulse)) * (tLambda > 0.0f ? 1.0f : -1.0f);
			contact._tangentImpulse += tLambda;
			this->impulse(contact, _tangentJacobians[i][j], _masses[i], tLambda);
		}
	}
}
#pragma endregion

#pragma region private
void Solver::impulse(const Contact2D& c, const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass, float lambda)
{
	float invMassA = std::get<0>(mass), invMassB = std::get<1>(mass), invInertiaA = std::get<2>(mass), invInertiaB = std::get<3>(mass);
	Vector2D dvA = { invMassA * jaco[0] * lambda, invMassA * jaco[1] * lambda };
	float dwA = invInertiaA * jaco[2] * lambda;
	Vector2D dvB = { invMassB * jaco[3] * lambda, invMassB * jaco[4] * lambda };
	float dwB = invInertiaB * jaco[5] * lambda;

	RigidBody2D* rigidA = c.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = c.colliderB()->attachedRigidBody();
	if (rigidA != nullptr)
	{
		rigidA->_velocity += dvA;
		rigidA->_angularVelocity += dwA;
	}
	if (rigidB != nullptr)
	{
		rigidB->_velocity += dvB;
		rigidB->_angularVelocity += dwB;
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
			if (rigidA != nullptr && rigidA->type() == E_BodyType::STATIC) // TODO : KINEMATIC도 적용시키나? ㄴㄴ 애초에 invMass, invInertia 0임.
			{
				rigidA->_velocity -= v * rigidA->invMass();
				rigidA->_angularVelocity -= Vector2D::cross(contact._rA, v) * rigidA->invInertia();
			}
			if (rigidB != nullptr && rigidB->type() == E_BodyType::STATIC)
			{
				rigidB->_velocity += v * rigidB->invMass();
				rigidB->_angularVelocity += Vector2D::cross(contact._rB, v) * rigidB->invInertia();
			}
		}
	}
}
float Solver::computePenetrationBias(const Contact2D& c, float dt)
{
	float depth = c.depth();
	return -(BAUMGART / dt) * std::max(depth - Solver::PENETRATION_SLOP, 0.0f);
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
	/*if (normalVel < 0.0f && -normalVel <= bouncinessThreshold)
		contact._bouncinessBias = 0.0f;
	else if (contact._bouncinessBias == 0.0f && normalVel < 0.0f && -normalVel > bouncinessThreshold)
		contact._bouncinessBias = bounciness * normalVel;*/
}
float Solver::computeEffectiveMass(const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass)
{
	float invMassA = std::get<0>(mass), invMassB = std::get<1>(mass), invInertiaA = std::get<2>(mass), invInertiaB = std::get<3>(mass);
	float val = (jaco[0] * jaco[0] + jaco[1] * jaco[1]) * invMassA + (jaco[2] * jaco[2]) * invInertiaA +
		(jaco[3] * jaco[3] + jaco[4] * jaco[4]) * invMassB + (jaco[5] * jaco[5]) * invInertiaB;
	return val > 0.0f ? 1.0f / val : 0.0f;
}
void Solver::getMassAndInertia(const Collision2D& c, float& invMassA, float& invMassB, float& invInertiaA, float& invInertiaB)
{
	RigidBody2D* rigidA = c.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = c.colliderB()->attachedRigidBody();

	if (rigidA == nullptr || rigidA->type() != E_BodyType::DYNAMIC)
	{
		invMassA = 0.0f;
		invInertiaA = 0.0f;
	}
	else
	{
		invMassA = rigidA->invMass();
		invInertiaA = rigidA->invInertia();
	}

	if (rigidB == nullptr || rigidB->type() != E_BodyType::DYNAMIC)
	{
		invMassB = 0.0f;
		invInertiaB = 0.0f;
	}
	else
	{
		invMassB = rigidB->invMass();
		invInertiaB = rigidB->invInertia();
	}
}
float Solver::computeLambda(const Contact2D& c, const std::vector<float>& jaco, float effMass, float bias)
{
	Vector2D vA, vB;
	float wA = 0.0f, wB = 0.0f;
	RigidBody2D* rigidA = c.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = c.colliderB()->attachedRigidBody();

	if (rigidA != nullptr)
	{
		vA = rigidA->velocity();
		wA = rigidA->angularVelocity();
	}
	if (rigidB != nullptr)
	{
		vB = rigidB->velocity();
		wB = rigidB->angularVelocity();
	}
	return -(jaco[0] * vA.x() + jaco[1] * vA.y() + jaco[2] * wA + jaco[3] * vB.x() + jaco[4] * vB.y() + jaco[5] * wB + bias) * effMass;
}
#pragma endregion

const float Solver::LINEAR_DAMPING = 0.98f;
const float Solver::ANGULAR_DAMPING = 0.98f;
const float Solver::PENETRATION_SLOP = 0.01f;