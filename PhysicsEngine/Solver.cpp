#include "pch.h"
#include "Solver.h"
#include "Collision2D.h"
#include "Vector2D.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "GameObject.h"

#pragma region public
Solver::Solver(const std::vector<Collision2D*>& collisions, float dt)
{
	_biases.resize(collisions.size());
	_normalJacobians.resize(collisions.size());
	_tangentJacobians.resize(collisions.size());
	_masses.resize(collisions.size());
	_normalEffMasses.resize(collisions.size());
	_tangentEffMasses.resize(collisions.size());
	for (int i = 0; i < collisions.size(); ++i)
	{
		Collision2D& c = *(collisions[i]);
		if (c._isTrigger) continue;

		this->computeBouncinessBias(c);
		_biases[i] = this->computePenetrationBias(c, dt);
		_normalJacobians[i] = { -c.normal().x(), -c.normal().y(), -Vector2D::cross(c._rA, c.normal()),
								c.normal().x(), c.normal().y(), Vector2D::cross(c._rB, c.normal()) };
		_tangentJacobians[i] = { -c.tangent().x(), -c.tangent().y(), -Vector2D::cross(c._rA, c.tangent()),
								c.tangent().x(), c.tangent().y(), Vector2D::cross(c._rB, c.tangent()) };
		this->getMassAndInertia(c, std::get<0>(_masses[i]), std::get<1>(_masses[i]), std::get<2>(_masses[i]), std::get<3>(_masses[i]));
		_normalEffMasses[i] = this->computeEffectiveMass(_normalJacobians[i], _masses[i]);
		_tangentEffMasses[i] = this->computeEffectiveMass(_tangentJacobians[i], _masses[i]);
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
		Collision2D& c = *(collisions[i]);
		if (c._isTrigger) continue;

		float nLambda = this->computeLambda(c, _normalJacobians[i], _normalEffMasses[i], _biases[i]);
		float tmp = c._normalImpulseSum;
		c._normalImpulseSum = std::max(tmp + nLambda, 0.0f);
		nLambda = c._normalImpulseSum - tmp;
		/*if (nLambda < 0.0f)
			nLambda = std::min(std::abs(nLambda), std::abs(c._normalImpulseSum)) * -1.0f;
		c._normalImpulseSum += nLambda;*/
		this->impulse(c, _normalJacobians[i], _masses[i], nLambda);

		float tLambda = this->computeLambda(c, _tangentJacobians[i], _tangentEffMasses[i], 0);
		float tmp2 = c._tangentImpulseSum;
		c._tangentImpulseSum = Utils::clamp(tmp2 + tLambda, c._friction * c._normalImpulseSum, -c._friction * c._normalImpulseSum);
		tLambda = c._tangentImpulseSum - tmp2;
		/*if (std::abs(c._tangentImpulseSum + tLambda) > c._friction * c._normalImpulseSum)
			tLambda -= (std::abs(c._tangentImpulseSum + tLambda) - (c._friction * c._normalImpulseSum)) * (tLambda > 0.0f ? 1.0f : -1.0f);
		c._tangentImpulseSum += tLambda;*/
		this->impulse(c, _tangentJacobians[i], _masses[i], tLambda);
	}
}
//void Solver::solvePositionConstraints(const std::vector<Collision2D*>& collisions, float dt)
//{
//	for (int i = 0; i < collisions.size(); ++i)
//	{
//		Collision2D& c = *(collisions[i]);
//		if (c._isTrigger) continue;
//
//		Point2D pA = c.colliderA()->toWorld(c.localContactA());
//		Point2D pB = c.colliderB()->toWorld(c.localContactB());
//		Vector2D rA = pA - c.colliderA()->position();
//		Vector2D rB = pB - c.colliderB()->position();
//		std::vector<float> jacobian = { -c.normal().x(), -c.normal().y(), -Vector2D::cross(rA, c.normal()),
//							c.normal().x(), c.normal().y(), Vector2D::cross(rB, c.normal()) };
//		float effMass = this->computeEffectiveMass(jacobian, _masses[i]);
//		float bias = this->computePenetrationBias(c, pA, pB, dt);
//		float lambda = this->computeLambda(c, { 0,0,0,0,0,0 }, effMass, bias); // TODO : 왜 {0,0,0,0,0,0}으로 주니까 되지?
//		this->impulse(c, jacobian, _masses[i], lambda);
//
//		RigidBody2D* rigidA = c.colliderA()->attachedRigidBody();
//		RigidBody2D* rigidB = c.colliderB()->attachedRigidBody();
//		if (rigidA != nullptr && rigidA->type() != E_BodyType::STATIC)
//		{
//			GameObject* obj = rigidA->gameObject();
//			Motion motion = { rigidA->velocity() * dt, rigidA->angularVelocity() * dt };
//			obj->processTransform(motion);
//		}
//		if (rigidB != nullptr && rigidB->type() != E_BodyType::STATIC)
//		{
//			GameObject* obj = rigidB->gameObject();
//			Motion motion = { rigidB->velocity() * dt, rigidB->angularVelocity() * dt };
//			obj->processTransform(motion);
//		}
//	}
//}
#pragma endregion

#pragma region private
void Solver::impulse(const Collision2D& c, const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass, float lambda)
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
float Solver::computePenetrationBias(const Collision2D& c, float dt)
{
	float depth = c.depth();
	return -(BAUMGART / dt) * std::max(depth - Solver::PENETRATION_SLOP, 0.0f);
	// b2Clamp(b2_baumgarte * (separation + b2_linearSlop), -b2_maxLinearCorrection, 0.0f)
	//return Utils::clamp(BAUMGART * (depth + Solver::PENETRATION_SLOP), 0.0f, -0.2f);
	//return -(BAUMGART / dt) * std::max(-depth - Solver::PENETRATION_SLOP, 0.0f);
}
void Solver::computeBouncinessBias(Collision2D& c)
{
	RigidBody2D* rigidA = c.colliderA()->attachedRigidBody();
	RigidBody2D* rigidB = c.colliderB()->attachedRigidBody();
	Vector2D relativeVel = {};
	float normalVel = 0.0f;

	if (rigidA != nullptr && rigidA->type() != E_BodyType::STATIC)
		relativeVel -= rigidA->velocity() + Vector2D::cross(rigidA->angularVelocity(), c._rA);
	if (rigidB != nullptr && rigidB->type() != E_BodyType::STATIC)
		relativeVel += rigidB->velocity() + Vector2D::cross(rigidB->angularVelocity(), c._rB);
	normalVel = Vector2D::dot(relativeVel, c.normal());

	if (normalVel < 0.0f && -normalVel <= c._bouncinessThreshold)
		c._bouncinessBias = 0.0f;
	else if (c._bouncinessBias == 0.0f && normalVel < 0.0f && -normalVel > c._bouncinessThreshold)
		c._bouncinessBias = c._bounciness * normalVel;
}
float Solver::computeEffectiveMass(const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass)
{
	float invMassA = std::get<0>(mass), invMassB = std::get<1>(mass), invInertiaA = std::get<2>(mass), invInertiaB = std::get<3>(mass);
	float val = (jaco[0] * jaco[0] + jaco[1] * jaco[1]) * invMassA + (jaco[2] * jaco[2]) * invInertiaA +
		(jaco[3] * jaco[3] + jaco[4] * jaco[4]) * invMassB + (jaco[5] * jaco[5]) * invInertiaB;
	return val > 0.0f ? 1 / val : 0.0f;
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
float Solver::computeLambda(const Collision2D& c, const std::vector<float>& jaco, float effMass, float bias)
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

const float Solver::PENETRATION_SLOP = 0.005f;