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
int World::addCollider(Collider2D* collider) { return s_broadPhase.insert(collider, collider->computeAABB()); }
void World::removeCollider(int key)
{
	Collider2D* collider = reinterpret_cast<Collider2D*>(s_broadPhase.getData(key));
	s_broadPhase.remove(key);
	s_detector.remove(collider);
}
void World::step(float dt)
{
	s_broadPhase.update();
	s_detector.update(s_broadPhase);
}
#pragma endregion

#pragma region private
#pragma endregion

Vector2D World::s_gravity;
BroadPhase World::s_broadPhase;
CollisionDetector World::s_detector;