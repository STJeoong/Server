#include "pch.h"
#include "World.h"
#include "Collider2D.h"
#include <ObjectPool.h>

#pragma region public
void World::init(const Vector2D& g)
{
	s_gravity = g;
	ObjectPool::makePool<Collision2D>(World::COLLISION_POOL_SIZE, []{ return new Collision2D(); },
		[](void* collision) {}, [](void* collision) { Collision2D* p = reinterpret_cast<Collision2D*>(collision); p->onDestroy(); });
	ObjectPool::makePool<Contact2D>(World::CONTACT_POOL_SIZE, [] { return new Contact2D(); },
		[](void* contact) {}, [](void* contact) {});
}
void World::addRigid(RigidBody2D* rigid) { s_rigids.push_back(rigid); }
void World::removeRigid(RigidBody2D* rigid)
{
	auto it = std::find(s_rigids.begin(), s_rigids.end(), rigid);
	s_rigids.erase(it);
}
size_t World::addCollider(Collider2D* collider) { return s_tree.insert(collider, collider->getAABB()); }
void World::removeCollider(size_t key)
{
	Collider2D* collider = reinterpret_cast<Collider2D*>(s_tree.getData(key));
	s_tree.remove(key);

	// TODO : need to be tested.
	auto it = s_collisions.begin();
	while (it < s_collisions.end())
	{
		it = std::find_if(it, s_collisions.end(), [&collider](Collision2D* c) { return c->colliderA() == collider || c->colliderB() == collider; });
		if (it != s_collisions.end())
		{
			ObjectPool::release(*it);
			it = s_collisions.erase(it);
		}
	}
}
void World::addJoint(Joint2D* joint) { s_joints.push_back(joint); }
void World::removeJoint(Joint2D* joint)
{
	auto it = std::find(s_joints.begin(), s_joints.end(), joint);
	s_joints.erase(it);
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
std::vector<Collision2D*> World::s_collisions;
DAT World::s_tree;