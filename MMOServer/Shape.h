#pragma once
#include "Game.h"
#include "MMO_struct.pb.h"

using namespace protocol::mmo;
class DAT;
class Area;

class Shape
{
	friend class Area;
public:
	virtual ~Shape() = default;
	virtual void removeFromDAT() = 0;
	virtual void insertToDAT(const TransformInt& pivot, void* userData) = 0;
	virtual void move(const TransformInt& pivot) = 0;
protected:
	int add(void* userData, const AABB& aabb) { return Game::add(userData, aabb); }
	void remove(int id) { Game::remove(id); }
	void move(int id, const AABB& newAABB) { Game::move(id, newAABB); }

	Area* _attachedArea = nullptr;
};