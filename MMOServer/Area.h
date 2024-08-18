#pragma once
#include "Behaviour.h"
#include "E_Layer.h"
#include "GameObject.h"
#include <vector>
#include <functional>

class Shape;
class Area : public Behaviour
{
public:
	void addShape(Shape* shape);
	bool detectMyArea() const { return _detectMyArea; }
	void detectMyArea(bool flag) { _detectMyArea = flag; }
	E_Layer layer() const { return _layer; }
	void layer(E_Layer val) { _layer = val; }
	bool fixedRotation() const { return _fixedRotation; }
	void fixedRotation(bool flag) { _fixedRotation = flag; }
	const std::vector<Area*>& overlappedAreas() const { return _overlappedAreas; }
protected:
	Area() { _oldTF = this->gameObject()->transform(); }
	~Area();
	virtual void update() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onDestroy() override;
	virtual void onAreaEnter(Area& my, Area& other) override;
	virtual void onAreaExit(Area& my, Area& other) override;
private:

	bool _detectMyArea = false;
	E_Layer _layer = E_Layer::DEFAULT;
	bool _fixedRotation = true;
	std::vector<Shape*> _shapes;
	TransformInt _oldTF;
	std::vector<Area*> _overlappedAreas;
};