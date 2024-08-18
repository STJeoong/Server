#include "Area.h"
#include "Shape.h"

#pragma region public
void Area::addShape(Shape* shape)
{
	_shapes.push_back(shape);
	shape->_attachedArea = this;
	if (!this->enable() || !this->gameObject()->activeInHierarchy())
		return;
	shape->insertToDAT(this->gameObject()->info().transform(), this);
}
#pragma endregion

#pragma region protected
Area::~Area()
{
	for (Shape* shape : _shapes)
		delete shape;
}
void Area::update()
{
	// 움직였으면 area 갱신
	const TransformInt& newTF = this->gameObject()->transform();
	if (_oldTF.y() == newTF.y() && _oldTF.x() == newTF.x() && (_fixedRotation || (_oldTF.dir() == newTF.dir())))
		return;

	for (Shape* shape : _shapes)
		shape->move(newTF);
	_oldTF = newTF;
}
void Area::onEnable()
{
	for (Shape* shape : _shapes)
		shape->insertToDAT(this->gameObject()->info().transform(), this);
	_oldTF = this->gameObject()->transform();
}
void Area::onDisable()
{
	for (Shape* shape : _shapes)
		shape->removeFromDAT();
}
void Area::onDestroy()
{
	for (Shape* shape : _shapes)
		shape->removeFromDAT();
}
void Area::onAreaEnter(Area& my, Area& other)
{
	_overlappedAreas.push_back(&other);
}
void Area::onAreaExit(Area& my, Area& other)
{
	_overlappedAreas.erase(std::find(_overlappedAreas.begin(), _overlappedAreas.end(), &other));
}
#pragma endregion

#pragma region private
#pragma endregion
