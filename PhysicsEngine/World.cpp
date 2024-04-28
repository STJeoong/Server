#include "pch.h"
#include "World.h"
#include "Collider2D.h"
#include "CollisionDetector.h"

#pragma region public
void World::addRigid(RigidBody2D* rigid)
{
	s_rigids.push_back(rigid);
	// TODO : broadphase�� �ش� rigidbody �߰�
}
void World::removeRigid(RigidBody2D* rigid)
{
	for (int i = 0; i < s_rigids.size(); ++i)
		if (s_rigids[i] == rigid)
		{
			s_rigids.erase(s_rigids.begin() + i);
			// TODO : broadphase���� �ش� rigidbody ����
			break;
		}
}
void World::addJoint(Joint2D* joint)
{
	s_joints.push_back(joint);
}
void World::removeJoint(Joint2D* joint)
{
	for (int i = 0; i < s_joints.size(); ++i)
		if (s_joints[i] == joint)
		{
			s_joints.erase(s_joints.begin() + i);
			break;
		}
}
void World::step(float dt)
{
	World::broadPhase();
}
#pragma endregion

#pragma region private
void World::broadPhase()
{
}
#pragma endregion

Vector2D World::s_gravity;
std::vector<RigidBody2D*> World::s_rigids;
std::vector<Joint2D*> World::s_joints;