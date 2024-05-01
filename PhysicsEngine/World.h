#pragma once
#include <vector>
#include "Vector2D.h"
#include "Collision2D.h"
#include "BroadPhase.h"
#include "CollisionDetector.h"

// 일단은 월드는 한개만 존재할 수 있음.
// simulate physics
class RigidBody2D;
class Joint2D;
class Collider2D;
class GameObject;
class World
{
	friend class Collider2D;
	static const size_t CONTACT_POOL_SIZE = 10000;
	static const size_t COLLISION_POOL_SIZE = 2000;
public:
	static GameObject* find(const std::string& name);
	static GameObject* instantiate(GameObject* obj = nullptr, GameObject* parent = nullptr, bool active = true);
	static void destroy(GameObject*& obj);

	static void init(const Vector2D& g);
	static void step(float dt);
private:
	static int add(Collider2D* collider);
	static void removalReq(int key); // request removal of collider
	static void invokeCollisionEvents();
	static void invokeExitEvents();
	static void destroyObjects();
	static void removeColliders();
	static void removeCollisions();
	static std::vector<Collision2D*>::iterator removeCollsion(std::vector<Collision2D*>::iterator& it);

	static std::vector<GameObject*> s_gameObjects;
	static std::vector<GameObject*> s_destroyed;
	static GameObject* s_root;

	static Vector2D s_gravity;
	static BroadPhase s_broadPhase;
	static CollisionDetector s_detector;
	static std::queue<int> s_removals;
	static std::vector<Collision2D*> s_exits;
};