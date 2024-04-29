#pragma once
#include <vector>
#include "Collision2D.h"

class BroadPhase;
class Collider2D;
class CollisionDetector
{
public:
	void remove(Collider2D* collider);
	void update(const BroadPhase& broadPhase);
	const std::vector<Collision2D*>& collisions() const { return _collisions; }
private:
	std::vector<Collision2D*> _collisions;
};