#pragma once
#include "MMO_struct.pb.h"

using namespace protocol::mmo;
class DAT;
class Area;
class AABB;
class Shape
{
public:
	Shape() = delete;
	Shape(Area* attahcedArea) : _attachedArea(attahcedArea) {}
	virtual ~Shape() = default;
	virtual void removeFromDAT() = 0;
	virtual void insertToDAT(const TransformInt& pivot, void* userData) = 0;
	virtual void move(const TransformInt& pivot) = 0;
	virtual Shape* clone(Area* attachedArea) = 0;
	bool overlaps(const Shape& other);
	
protected:
	int add(void* userData, const AABB& aabb);
	void remove(int id);
	void move(int id, const AABB& newAABB);

	Area* _attachedArea = nullptr;
	std::vector<int> _ids;
};