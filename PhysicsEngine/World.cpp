#include "pch.h"
#include "World.h"
#include "Body.h"
#include "Collider2D.h"
#include "CollisionDetector.h"

#pragma region public
void World::broadPhase()
{
	for (int i = 0; i < _rigids.size(); ++i)
		for (int j = i + 1; j < _rigids.size(); ++j)
		{
			if (CollisionDetector::detect(_rigids[i]->collider()->getAABB(), _rigids[j]->collider()->getAABB()))
			{
				ArbiterKey key{ _rigids[i], _rigids[j] };
				//if (_arbiters.find(key) == _arbiters.end())
				//	_arbiters[key] = { _rigids[i], _rigids[j] };
				//else; // update arbiter
			}
			else
			{
				ArbiterKey key{ _rigids[i], _rigids[j] };
				/*if (_arbiters.find(key) != _arbiters.end())
					_arbiters.erase(key);*/
			}
		}
}
void World::step(float dt)
{
	this->broadPhase();
}
#pragma endregion

#pragma region private
#pragma endregion