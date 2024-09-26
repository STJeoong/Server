#include "Rectangular.h"
#include "Area.h"
#include "Utils.h"

#pragma region public
void Rectangular::removeFromDAT()
{
	if (_ids[0] == DAT::NULL_NODE)
		return;
	this->remove(_ids[0]);
	_ids[0] = DAT::NULL_NODE;
}
void Rectangular::insertToDAT(const TransformInt& pivot, Area* userData)
{
	AABB aabb = Utils::calcAABB(_rect, pivot, _attachedArea->fixedRotation());
	_ids[0] = this->add(userData, aabb);
}
void Rectangular::move(const TransformInt& pivot)
{
	AABB aabb = Utils::calcAABB(_rect, pivot, _attachedArea->fixedRotation());
	Shape::move(_ids[0], aabb);
}
#pragma endregion

#pragma region private
#pragma endregion
