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
void Rectangular::insertToDAT(const TransformInt& pivot, void* userData)
{
	/*int leftTopY = pivot.y() + _offsetY;
	int leftTopX = pivot.x() + _offsetX;
	int rightBtmY = pivot.y() + _offsetY + _yExtension;
	int rightBtmX = pivot.x() + _offsetX + _xExtension;*/

	int leftTopY = pivot.y() + _offsetY - _yExtension;
	int leftTopX = pivot.x() + _offsetX - _xExtension;
	int rightBtmY = pivot.y() + _offsetY + _yExtension;
	int rightBtmX = pivot.x() + _offsetX + _xExtension;
	if (!_attachedArea->fixedRotation())
	{
		TransformInt localOffset, localExtension;
		TransformInt worldOffset, worldExtension;

		localOffset.set_dir(E_Dir::BOTTOM);
		localOffset.set_y(_offsetY);
		localOffset.set_x(_offsetX);
		localExtension.set_dir(E_Dir::BOTTOM);
		localExtension.set_y(_yExtension);
		localExtension.set_x(_xExtension);
		Utils::localToWorld(pivot, localOffset, worldOffset);
		Utils::localToWorld(pivot, localExtension, worldExtension);

		leftTopY = pivot.y() + worldOffset.y() - std::abs(worldExtension.y());
		leftTopX = pivot.x() + worldOffset.x() - std::abs(worldExtension.x());
		rightBtmY = pivot.y() + worldOffset.y() + std::abs(worldExtension.y());
		rightBtmX = pivot.x() + worldOffset.x() + std::abs(worldExtension.x());
	}

	_ids[0] = this->add(userData, { leftTopY, leftTopX, rightBtmY, rightBtmX });
}
void Rectangular::move(const TransformInt& pivot)
{
	int leftTopY = pivot.y() + _offsetY - _yExtension;
	int leftTopX = pivot.x() + _offsetX - _xExtension;
	int rightBtmY = pivot.y() + _offsetY + _yExtension;
	int rightBtmX = pivot.x() + _offsetX + _xExtension;
	if (!_attachedArea->fixedRotation())
	{
		TransformInt localOffset, localExtension;
		TransformInt worldOffset, worldExtension;

		localOffset.set_dir(E_Dir::BOTTOM);
		localOffset.set_y(_offsetY);
		localOffset.set_x(_offsetX);
		localExtension.set_dir(E_Dir::BOTTOM);
		localExtension.set_y(_yExtension);
		localExtension.set_x(_xExtension);
		Utils::localToWorld(pivot, localOffset, worldOffset);
		Utils::localToWorld(pivot, localExtension, worldExtension);

		leftTopY = pivot.y() + worldOffset.y() - std::abs(worldExtension.y());
		leftTopX = pivot.x() + worldOffset.x() - std::abs(worldExtension.x());
		rightBtmY = pivot.y() + worldOffset.y() + std::abs(worldExtension.y());
		rightBtmX = pivot.x() + worldOffset.x() + std::abs(worldExtension.x());
	}

	Shape::move(_ids[0], { leftTopY, leftTopX, rightBtmY, rightBtmX });
}
#pragma endregion

#pragma region private
#pragma endregion
