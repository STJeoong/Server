#pragma once
#include <vector>
#include "Collision2D.h"

class BroadPhase;
class Collider2D;
class CollisionDetector
{
public:
	void findRelatedCollisions(Collider2D* collider, std::vector<Collision2D*>& exits);
	void update(const BroadPhase& broadPhase);
	const std::vector<Collision2D*>& collisions() const { return _collisions; }
private:
	void pruneOldCollisions(const BroadPhase& broadPhase);
	void importFromBroadPhase(const BroadPhase& broadPhase);
	void narrowPhase(Collision2D* collision);
	bool gjk(Collision2D* collision);
	void epa(Collision2D* collision);
	void setExit(Collision2D* collision);

	std::vector<Collision2D*> _collisions;
};