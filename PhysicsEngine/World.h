#pragma once
#include <vector>
#include "Vector2D.h"
#include "RigidBody2D.h"
#include "Joint2D.h"

// 일단은 월드는 한개만 존재할 수 있음.
// simulate physics
class World
{
public:
	static void gravity(const Vector2D& g) { s_gravity = g; }
	static void addRigid(RigidBody2D* rigid);
	static void removeRigid(RigidBody2D* rigid);
	static void addJoint(Joint2D* joint);
	static void removeJoint(Joint2D* joint);
	static void step(float dt);
private:
	static void broadPhase();

	static Vector2D s_gravity;
	static std::vector<RigidBody2D*> s_rigids;
	static std::vector<Joint2D*> s_joints;
};