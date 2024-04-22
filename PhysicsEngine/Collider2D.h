#pragma once

class AABB;
class Collider2D
{
public:
	virtual ~Collider2D() = default;
	virtual AABB getAABB() = 0;
};