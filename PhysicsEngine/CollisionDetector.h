#pragma once
#include "AABB.h"

class CollisionDetector
{
public:
	static bool detect(const AABB& a, const AABB& b);
};