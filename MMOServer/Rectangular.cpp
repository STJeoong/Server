#include "Rectangular.h"
#include "Area.h"

#pragma region public
void Rectangular::removeFromDAT()
{
	if (_id == DAT::NULL_NODE)
		return;
	this->remove(_id);
	_id = DAT::NULL_NODE;
}
void Rectangular::insertToDAT(const TransformInt& pivot, void* userData)
{
	// TODO
	int leftTopY = pivot.y() + _offsetY - _yExtension;
	int leftTopX = pivot.x() + _offsetX - _xExtension;
	int rightBtmY = pivot.y() + _offsetY + _yExtension;
	int rightBtmX = pivot.x() + _offsetX + _xExtension;
	if (!_attachedArea->fixedRotation())
	{
	}

	this->add(userData, { leftTopY, leftTopX, rightBtmY, rightBtmX });
}
void Rectangular::move(const TransformInt& pivot)
{
	int leftTopY = pivot.y() - _yExtension;
	int leftTopX = pivot.x() - _xExtension;
	int rightBtmY = pivot.y() + _yExtension;
	int rightBtmX = pivot.x() + _xExtension;
	if (!_attachedArea->fixedRotation())
	{

	}

	Shape::move(_id, { leftTopY, leftTopX, rightBtmY, rightBtmX });
}
#pragma endregion

#pragma region private
#pragma endregion
