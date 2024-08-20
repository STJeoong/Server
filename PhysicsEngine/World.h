#pragma once
#include <vector>
#include "Vector2D.h"
#include "Collision2D.h"
#include "BroadPhase.h"
#include "CollisionDetector.h"
#include "Solver.h"

// 일단은 월드는 한개만 존재할 수 있음.
// simulate physics
class RigidBody2D;
class Joint2D;
class Collider2D;
class GameObject;
class World
{
	friend class Collider2D;
	friend class RigidBody2D;
	static const size_t COLLISION_POOL_SIZE = 5000;
	static const size_t CONTACT_POOL_SIZE = 10000;
public:
	static GameObject* find(const std::string& name);
	static GameObject* instantiate(GameObject* copy = nullptr, GameObject* parent = nullptr, bool active = true);
	static void destroy(GameObject*& obj);

	static void init(const Vector2D& g);
	static void step(float dt, int velocityIter = 8, int positionIter = 3);
private:
	static int add(Collider2D* collider);
	static void add(RigidBody2D* rigid);
	static void moveCollider(int key, const AABB& sweepAABB, const Vector2D& displacement);
	static void removeRigid(RigidBody2D* rigid);
	static void removalReq(int key); // request removal of collider
	static void invokeCollisionEvents();
	static void invokeExitEvents();
	static void destroyObjects();
	static void removeColliders();
	static void removeCollisions();
	static std::vector<Collision2D*>::iterator removeCollsion(std::vector<Collision2D*>::iterator& it);

	static std::vector<GameObject*> s_gameObjects;
	static std::vector<GameObject*> s_destroyed;
	static GameObject* s_root; // 모든 물체를 한번에 이동할 때 사용할 수 있음.

	static Vector2D s_gravity;
	static BroadPhase s_broadPhase;
	static std::vector<RigidBody2D*> s_rigids;
	static CollisionDetector s_detector;
	static Solver s_solver;
	static std::queue<int> s_removals;
	static std::vector<Collision2D*> s_exits;
};