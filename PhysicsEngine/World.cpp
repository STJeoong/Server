#include "pch.h"
#include "World.h"
#include "Collider2D.h"
#include "GameObject.h"
#include <ObjectPool.h>

#pragma region public
GameObject* World::find(const std::string& name)
{
	for (GameObject*& obj : s_gameObjects)
		if (obj->_name == name)
			return obj;
	return nullptr;
}
GameObject* World::instantiate(GameObject* obj, GameObject* parent, bool active)
{
	if (s_root == nullptr) s_root = new GameObject();
	GameObject* ret;
	if (obj == nullptr) ret = new GameObject();
	else ret = new GameObject(*obj);
	ret->_isActive = active;
	if (parent == nullptr) ret->setParent(*s_root);
	else ret->setParent(*parent);
	s_root->_children.push_back(ret);
	s_gameObjects.push_back(ret);
	return ret;
}
void World::destroy(GameObject*& obj)
{
	auto it = std::find(s_gameObjects.begin(), s_gameObjects.end(), obj);
	s_gameObjects.erase(it);
	obj->broadcast(E_GameObjectEvent::DESTROY, nullptr);
	delete obj;
	obj = nullptr;
}

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

std::vector<GameObject*> World::s_gameObjects;
GameObject* World::s_root;

Vector2D World::s_gravity;
BroadPhase World::s_broadPhase;
CollisionDetector World::s_detector;