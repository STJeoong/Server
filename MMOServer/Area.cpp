#include "Area.h"
#include "Shape.h"

#pragma region public
void Area::addShape(Shape* shape)
{
	_shapes.push_back(shape);
	shape->_attachedArea = this;
	if (!this->enable() || !_gameObject->activeInHierarchy())
		return;
	shape->insertToDAT(_gameObject->transform(), this);
}
bool Area::overlaps(const Area& other) const
{
	for (int i = 0; i < _shapes.size(); ++i)
		for (int j = 0; j < other._shapes.size(); ++j)
			if (_shapes[i]->overlaps(*other._shapes[j]))
				return true;
	return false;
}
#pragma endregion

#pragma region protected
Area::~Area()
{
	for (Shape* shape : _shapes)
		delete shape;
}
void Area::awake()
{
	_oldTF = _gameObject->transform();
}
void Area::update()
{
	// 움직였으면 area 갱신
	const TransformInt& newTF = _gameObject->transform();
	if (_oldTF.y() == newTF.y() && _oldTF.x() == newTF.x() && (_fixedRotation || (_oldTF.dir() == newTF.dir())))
		return;
	
	for (Shape* shape : _shapes)
		shape->move(newTF);
	_oldTF = newTF;
}
void Area::onEnable()
{
	for (Shape* shape : _shapes)
		shape->insertToDAT(_gameObject->transform(), this);
	_oldTF = _gameObject->transform();
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
	if (this != &my) return;
	_overlappedAreas.push_back(&other);
}
void Area::onAreaExit(Area& my, Area& other)
{
	if (this != &my) return;
	_overlappedAreas.erase(std::find(_overlappedAreas.begin(), _overlappedAreas.end(), &other));
}
#pragma endregion

#pragma region private
void Area::copyTo(Component* instance)
{
	Area* area = reinterpret_cast<Area*>(instance);
	area->_detectMyArea = _detectMyArea;
	area->_layer = _layer;
	area->_fixedRotation = _fixedRotation;
	area->_enabled = _enabled;
	for (Shape* shape : _shapes)
		area->addShape(shape->clone());
}
#pragma endregion
