#pragma once
#include <vector>
#include "Contact2D.h"
#include "E_GameObjectEvent.h"
#include "Simplex.h"
class Collision2D
{
	friend class World;
	friend class CollisionDetector;
public:
	size_t contactCount() const { return _contacts.size(); }
	const Contact2D& getContact(size_t idx) const { return *_contacts[idx]; }
	Collider2D* const& colliderA() const { return _colliderA; }
	Collider2D* const& colliderB() const { return _colliderB; }
	RigidBody2D* const& rigidA() const { return _rigidA; }
	RigidBody2D* const& rigidB() const { return _rigidB; }
private:
	void onDestroy();

	std::vector<Contact2D*> _contacts;
	Collider2D* _colliderA = nullptr;
	Collider2D* _colliderB = nullptr;
	RigidBody2D* _rigidA = nullptr;
	RigidBody2D* _rigidB = nullptr;
	bool _isTouching = false;
	Simplex _simplex;
	E_GameObjectEvent _evt = E_GameObjectEvent::NONE;
};