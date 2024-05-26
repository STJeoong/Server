#pragma once
#include <vector>
#include "E_GameObjectEvent.h"
#include "Simplex.h"
class Collider2D;
class RigidBody2D;
class Contact2D;
class Collision2D
{
	friend class World;
	friend class Solver;
	friend class CollisionDetector;
public:
	bool isTrigger() const { return _isTrigger; }
	Collider2D* colliderA() const { return _colliderA; }
	Collider2D* colliderB() const { return _colliderB; }
	const std::vector<Contact2D*>& contacts() const { return _contacts; }
private:
	void validateOldContacts(); // 중복된 점, depth < -2.0f * MARGIN인 점들 모두 삭제
	bool importNewContact(Contact2D* contact); // 기존의 contact와 다른 contact만 추가
	void prune();

	void updateContacts();
	void computeBouncinessBias();

	void clearContacts();
	void onDestroy();


	Collider2D* _colliderA;
	Collider2D* _colliderB;
	std::vector<Contact2D*> _contacts;
	float _bounciness = 0.0f;
	float _bouncinessThreshold = 1.0f;
	float _friction = 0.0f;
	bool _isTrigger = false;

	Simplex _simplex;
	E_GameObjectEvent _evt = E_GameObjectEvent::NONE;
};