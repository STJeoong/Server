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
Solver::Solver(const std::vector<Collision2D*>& collisions)
{
	this->warmStart(collisions);
	_velocityBiases.resize(collisions.size());
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
		_velocityBiases[i].resize(contacts.size());
		_normalJacobians[i].resize(contacts.size());
		_tangentJacobians[i].resize(contacts.size());
		_normalEffMasses[i].resize(contacts.size());
		_tangentEffMasses[i].resize(contacts.size());
		for (int j = 0; j < contacts.size(); ++j)
		{
			Contact2D& contact = *(contacts[j]);
			this->computeBouncinessBias(contact, collision._bounciness, collision._bouncinessThreshold);
			_velocityBiases[i][j] = contact._bouncinessBias;
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

			float tLambda = this->computeLambda(contact, _tangentJacobians[i][j], _tangentEffMasses[i][j], 0);
			float oldTangentImpulse = contact._tangentImpulse;
			float maxTangentImpulse = contact._normalImpulse * collision._friction;
			contact._tangentImpulse = Utils::clamp(oldTangentImpulse + tLambda, maxTangentImpulse, -maxTangentImpulse);
			tLambda = contact._tangentImpulse - oldTangentImpulse;
			this->impulse(contact, _tangentJacobians[i][j], _masses[i], tLambda);

			float nLambda = this->computeLambda(contact, _normalJacobians[i][j], _normalEffMasses[i][j], _velocityBiases[i][j]);
			if (nLambda < 0.0f)
				nLambda = -std::min(std::abs(nLambda), contact._normalImpulse);
			contact._normalImpulse += nLambda;
			this->impulse(contact, _normalJacobians[i][j], _masses[i], nLambda);
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

			Collider2D* colliderA = contact.colliderA();
			Collider2D* colliderB = contact.colliderB();
			Point2D globalPointA = colliderA->toWorld(contact.localContactA());
			Point2D globalPointB = colliderB->toWorld(contact.localContactB());
			Vector2D rA = globalPointA - colliderA->position();
			Vector2D rB = globalPointB - colliderB->position();
			/*Vector2D normal;
			if (contact._isEdgeA)
				normal = Matrix22(colliderA->gameObject()->transform().rotation().radian() - contact._rotationA) * contact.normal();
			else if (contact._isEdgeB)
				normal = Matrix22(colliderB->gameObject()->transform().rotation().radian() - contact._rotationB) * contact.normal();
			std::vector<float> jacobian = { -normal.x(), -normal.y(), -Vector2D::cross(rA, normal),
						normal.x(), normal.y(), Vector2D::cross(rB, normal) };*/
			std::vector<float> jacobian = { -contact.normal().x(), -contact.normal().y(), -Vector2D::cross(rA, contact.normal()),
						contact.normal().x(), contact.normal().y(), Vector2D::cross(rB, contact.normal()) };
			float effMass = this->computeEffectiveMass(jacobian, _masses[i]);
			float depth = Vector2D::dot(globalPointA - globalPointB, contact.normal());
			float bias = this->computePenetrationBias(contact, depth);
			float nLambda = -bias * effMass;
			
			GameObject* objA = colliderA->gameObject();
			GameObject* objB = colliderB->gameObject();
			float invMassA = std::get<0>(_masses[i]), invMassB = std::get<1>(_masses[i]);
			float invInertiaA = std::get<2>(_masses[i]), invInertiaB = std::get<3>(_masses[i]);

			{
				Vector2D dv = Vector2D{ invMassA * jacobian[0] * nLambda, invMassA * jacobian[1] * nLambda };
				float dw = invInertiaA * jacobian[2] * nLambda;
				Motion motion = { dv, dw };
				objA->processTransform(motion);
			}
			{
				Vector2D dv = Vector2D{ invMassB * jacobian[3] * nLambda, invMassB * jacobian[4] * nLambda };
				float dw = invInertiaB * jacobian[5] * nLambda;
				Motion motion = { dv, dw };
				objB->processTransform(motion);
			}

			/*{
				Vector2D dv = Vector2D{ invMassA * _normalJacobians[i][j][0] * nLambda, invMassA * _normalJacobians[i][j][1] * nLambda };
				float dw = invInertiaA * _normalJacobians[i][j][2] * nLambda;
				Motion motion = { dv, dw };
				objA->processTransform(motion);
			}
			{
				Vector2D dv = Vector2D{ invMassB * _normalJacobians[i][j][3] * nLambda, invMassB * _normalJacobians[i][j][4] * nLambda };
				float dw = invInertiaB * _normalJacobians[i][j][5] * nLambda;
				Motion motion = { dv, dw };
				objB->processTransform(motion);
			}*/
		}
	}
}
#pragma endregion

#pragma region private
void Solver::impulse(const Contact2D& contact, const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass, float lambda)
{
	float invMassA = std::get<0>(mass), invMassB = std::get<1>(mass), invInertiaA = std::get<2>(mass), invInertiaB = std::get<3>(mass);
	Vector2D dvA = Vector2D{ invMassA * jaco[0] * lambda, invMassA * jaco[1] * lambda };
	float dwA = invInertiaA * jaco[2] * lambda;
	Vector2D dvB = Vector2D{ invMassB * jaco[3] * lambda, invMassB * jaco[4] * lambda };
	float dwB = invInertiaB * jaco[5] * lambda;

	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();
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
	float slop = Solver::PENETRATION_SLOP * std::min(colliderA->size(), colliderB->size());
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
float Solver::computeEffectiveMass(const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass)
{
	float invMassA = std::get<0>(mass), invMassB = std::get<1>(mass), invInertiaA = std::get<2>(mass), invInertiaB = std::get<3>(mass);
	float effMass = (jaco[0] * jaco[0] + jaco[1] * jaco[1]) * invMassA + (jaco[2] * jaco[2]) * invInertiaA +
		(jaco[3] * jaco[3] + jaco[4] * jaco[4]) * invMassB + (jaco[5] * jaco[5]) * invInertiaB;
	return effMass > 0.0f ? 1.0f / effMass : 0.0f;
}
void Solver::getMassAndInertia(const Collision2D& collision, float& invMassA, float& invMassB, float& invInertiaA, float& invInertiaB)
{
	RigidBody2D* rigidA = collision.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = collision.colliderB()->attachedRigidBody();

	invMassA = rigidA == nullptr ? 0.0f : rigidA->invMass();
	invMassB = rigidB == nullptr ? 0.0f : rigidB->invMass();
	invInertiaA = rigidA == nullptr ? 0.0f : rigidA->invInertia();
	invInertiaB = rigidB == nullptr ? 0.0f : rigidB->invInertia();
}
float Solver::computeLambda(const Contact2D& contact, const std::vector<float>& jaco, float effMass, float bias)
{
	Vector2D vA, vB;
	float wA = 0.0f, wB = 0.0f;
	RigidBody2D* rigidA = contact.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = contact.colliderB()->attachedRigidBody();

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

const float Solver::PENETRATION_SLOP = 0.01f;