#pragma once
#include "Vector2D.h"
#include "Body.h"
#include <vector>
#include <unordered_map>

class World
{
public:
	World(const Vector2D& gravity) : _gravity(gravity) {}
	void add(Body* rigid) { _rigids.push_back(rigid); }
	void broadPhase();
	void step(float dt);
private:

	Vector2D _gravity;
	std::vector<Body*> _rigids;
	//std::unordered_map<ArbiterKey, ArbPair> _arbiters;
};