#include "Shape.h"
#include "Map.h"
#include "Area.h"
#include "DAT.h"
#include "GameObject.h"

#pragma region public
bool Shape::overlaps(const Shape& other)
{
	Map* mapA = _attachedArea->gameObject()->map();
	Map* mapB = other._attachedArea->gameObject()->map();
	if (mapA != mapB) return false;

	for (int i = 0; i < _ids.size(); ++i)
		for (int j = 0; j < other._ids.size(); ++j)
			if (mapA->_dat->getAABB(_ids[i]).overlaps(mapB->_dat->getAABB(other._ids[j])))
				return true;
	return false;
}
#pragma endregion

#pragma region protected
int Shape::add(void* userData, const AABB& aabb)
{
	return _attachedArea->gameObject()->map()->add(userData, aabb);
}
void Shape::remove(int id)
{
	_attachedArea->gameObject()->map()->remove(id);
}
void Shape::move(int id, const AABB& newAABB)
{
	_attachedArea->gameObject()->map()->move(id, newAABB);
}
#pragma endregion

#pragma region private
#pragma endregion