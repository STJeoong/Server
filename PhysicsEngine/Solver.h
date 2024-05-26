#pragma once
#include <vector>
#include "Vector2D.h"
class Collision2D;
class RigidBody2D;
class Contact2D;
class Solver
{
	static const float MAX_LINEAR_CORRECTION;
	static const float PENETRATION_SLOP;
public:
	void init(const std::vector<Collision2D*>& collisions);
	void integrateVelocity(const std::vector<RigidBody2D*>& rigids, const Vector2D& g, float dt);
	void integratePosition(const std::vector<RigidBody2D*>& rigids, float dt);
	void solveVelocityConstraints(const std::vector<Collision2D*>& collisions);
	void solvePositionConstraints(const std::vector<Collision2D*>& collisions);
private:
	void impulse(const Contact2D& contact, const Vector2D& dir, float lambda);
	void warmStart(const std::vector<Collision2D*>& collisions);
	float computePenetrationBias(float depth);
	float computeEffectiveMass(const Contact2D& contact, const Vector2D& dir);
	float computeJV(const Contact2D& contact, const Vector2D& dir);

	std::vector<std::vector<float>> _normalEffMasses;
	std::vector<std::vector<float>> _tangentEffMasses;
};