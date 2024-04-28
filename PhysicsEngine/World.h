#pragma once
#include <vector>
#include "Vector2D.h"
#include "Collision2D.h"
#include "DAT.h"

// 일단은 월드는 한개만 존재할 수 있음.
// simulate physics
class RigidBody2D;
class Joint2D;
class Collider2D;
class World
{
	static const size_t CONTACT_POOL_SIZE = 10000;
	static const size_t COLLISION_POOL_SIZE = 2000;
public:
	static void init(const Vector2D& g);
	static int addCollider(Collider2D* collider);
	static void removeCollider(int key);
	static void addJoint(Joint2D* joint);
	static void removeJoint(Joint2D* joint);
	static void step(float dt);
private:
	static void broadPhase();

	static Vector2D s_gravity;
	static std::vector<Joint2D*> s_joints;
	static std::vector<Collision2D*> s_collisions;
	static DAT s_tree;
};