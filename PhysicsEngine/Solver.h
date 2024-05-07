#pragma once
#include <vector>
class Collision2D;
class RigidBody2D;
class Vector2D;
class Solver
{
public:
	void integrateVelocity(const std::vector<RigidBody2D*>& rigids, const Vector2D& g, float dt);
	void integratePosition(const std::vector<RigidBody2D*>& rigids, float dt);
	void solveVelocityConstraints(const std::vector<Collision2D*>& collisions);
	void solvePositionConstraints(const std::vector<Collision2D*>& collisions);
private:
	void init(const std::vector<Collision2D*>& collisions);
	void normalImpulse(const Collision2D& c, const std::vector<float>& jaco, const std::tuple<float, float, float, float>& mass, float lambda);
	float computeBias(const Collision2D& c);
	float computeEffectiveMass(const std::vector<float>& jaco, float invMassA, float invMassB, float invInertiaA, float invInertiaB);
	void getMassAndInertia(const Collision2D& c, float& invMassA, float& invMassB, float& invInertiaA, float& invInertiaB);
	float computeLambda(const Collision2D& c, const std::vector<float>& jaco, float effMass, float bias);

	bool _isFirstVelocityIter = true;
	std::vector<float> _biases;
	std::vector<float> _effMasses;
	std::vector<std::vector<float>> _jacobians;
	std::vector<std::tuple<float, float, float, float>> _masses; // invMassA, invMassB, invInertiaA, invInertiaB
};