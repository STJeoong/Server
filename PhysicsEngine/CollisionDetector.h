#pragma once
#include <vector>
#include "Collision2D.h"

class BroadPhase;
class Collider2D;
class CollisionDetector
{
public:
	void remove(Collider2D* collider);
	void remove(Collision2D* collision);
	void update(const BroadPhase& broadPhase);
	const std::vector<Collision2D*>& collisions() const { return _collisions; }
private:
	void removeOldCollisions(const BroadPhase& broadPhase);
	void importFromBroadPhase(const BroadPhase& broadPhase);
	void update(Collision2D* collision);
	bool gjk(Collider2D* colliderA, Collider2D* colliderB);

	std::vector<Collision2D*> _collisions;
};