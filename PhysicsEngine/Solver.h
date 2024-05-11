#pragma once
#include <vector>
#include "Vector2D.h"
class Collision2D;
class RigidBody2D;
class Contact2D;
class Solver
{
	static const float PENETRATION_SLOP;
public:
	Solver(const std::vector<Collision2D*>& collisions, float dt);
	void integrateVelocity(const std::vector<RigidBody2D*>& rigids, const Vector2D& g, float dt);
	void integratePosition(const std::vector<RigidBody2D*>& rigids, float dt);
	void resolve(const std::vector<Collision2D*>& collisions);
private:
	void impulse(const Contact2D& contact, const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass, float lambda);
	void warmStart(const std::vector<Collision2D*>& collisions);
	float computePenetrationBias(const Contact2D& contact, float dt);
	void computeBouncinessBias(Contact2D& contact, float bounciness, float bouncinessThreshold);
	float computeEffectiveMass(const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass);
	void getMassAndInertia(const Collision2D& c, float& invMassA, float& invMassB, float& invInertiaA, float& invInertiaB);
	float computeLambda(const Contact2D& c, const std::vector<float>& jaco, float effMass, float bias);

	std::vector<std::vector<float>> _biases;
	std::vector<std::vector<float>> _normalEffMasses;
	std::vector<std::vector<float>> _tangentEffMasses;
	std::vector<std::vector<std::vector<float>>> _normalJacobians;
	std::vector<std::vector<std::vector<float>>> _tangentJacobians;
	std::vector<std::tuple<float, float, float, float>> _masses; // invMassA, invMassB, invInertiaA, invInertiaB
};