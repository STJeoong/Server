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
	int leftTopY = pivot.y() + _offsetY;
	int leftTopX = pivot.x() + _offsetX;
	int rightBtmY = pivot.y() + _offsetY + _yExtension;
	int rightBtmX = pivot.x() + _offsetX + _xExtension;
	if (!_attachedArea->fixedRotation())
	{
		TransformInt localPoints[4] = {};
		TransformInt worldPoints[4] = {};

		//  로컬 직사각형의 좌표
		//  [0]  [1]
		//  [2]  [3]
		localPoints[0].set_dir(E_Dir::BOTTOM);
		localPoints[0].set_y(_offsetY);
		localPoints[0].set_x(_offsetX);

		localPoints[1].set_dir(E_Dir::BOTTOM);
		localPoints[1].set_y(_offsetY);
		localPoints[1].set_x(_offsetX + _xExtension);
		
		localPoints[2].set_dir(E_Dir::BOTTOM);
		localPoints[2].set_y(_offsetY + _yExtension);
		localPoints[2].set_x(_offsetX);

		localPoints[3].set_dir(E_Dir::BOTTOM);
		localPoints[3].set_y(_offsetY + _yExtension);
		localPoints[3].set_x(_offsetX + _xExtension);
		for (int i = 0; i < 4; ++i)
			Utils::localToWorld(pivot, localPoints[i], worldPoints[i]);

		leftTopY = worldPoints[0].y();
		leftTopX = worldPoints[0].x();
		rightBtmY = worldPoints[0].y();
		rightBtmX = worldPoints[0].x();
		for (int i = 1; i < 4; ++i)
		{
			leftTopY = std::min(leftTopY, worldPoints[i].y());
			leftTopX = std::min(leftTopX, worldPoints[i].x());
			rightBtmY = std::max(rightBtmY, worldPoints[i].y());
			rightBtmX = std::max(rightBtmX, worldPoints[i].x());
		}
	}

	_ids[0] = this->add(userData, { leftTopY, leftTopX, rightBtmY, rightBtmX });
}
void Rectangular::move(const TransformInt& pivot)
{
	int leftTopY = pivot.y() + _offsetY;
	int leftTopX = pivot.x() + _offsetX;
	int rightBtmY = pivot.y() + _offsetY + _yExtension;
	int rightBtmX = pivot.x() + _offsetX + _xExtension;
	if (!_attachedArea->fixedRotation())
	{
		TransformInt localPoints[4] = {};
		TransformInt worldPoints[4] = {};

		//  로컬 직사각형의 좌표
		//  [0]  [1]
		//  [2]  [3]
		localPoints[0].set_dir(E_Dir::BOTTOM);
		localPoints[0].set_y(_offsetY);
		localPoints[0].set_x(_offsetX);

		localPoints[1].set_dir(E_Dir::BOTTOM);
		localPoints[1].set_y(_offsetY);
		localPoints[1].set_x(_offsetX + _xExtension);

		localPoints[2].set_dir(E_Dir::BOTTOM);
		localPoints[2].set_y(_offsetY + _yExtension);
		localPoints[2].set_x(_offsetX);

		localPoints[3].set_dir(E_Dir::BOTTOM);
		localPoints[3].set_y(_offsetY + _yExtension);
		localPoints[3].set_x(_offsetX + _xExtension);
		for (int i = 0; i < 4; ++i)
			Utils::localToWorld(pivot, localPoints[i], worldPoints[i]);

		leftTopY = worldPoints[0].y();
		leftTopX = worldPoints[0].x();
		rightBtmY = worldPoints[0].y();
		rightBtmX = worldPoints[0].x();
		for (int i = 1; i < 4; ++i)
		{
			leftTopY = std::min(leftTopY, worldPoints[i].y());
			leftTopX = std::min(leftTopX, worldPoints[i].x());
			rightBtmY = std::max(rightBtmY, worldPoints[i].y());
			rightBtmX = std::max(rightBtmX, worldPoints[i].x());
		}
	}

	Shape::move(_ids[0], { leftTopY, leftTopX, rightBtmY, rightBtmX });
}
#pragma endregion

#pragma region private
#pragma endregion
