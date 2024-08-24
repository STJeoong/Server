#pragma once
#include "Behaviour.h"
#include "E_Layer.h"
#include "GameObject.h"
#include <vector>
#include <functional>

class Shape;
class Area : public Behaviour
{
	friend class GameObject;
public:
	void addShape(Shape* shape);
	bool detectMyArea() const { return _detectMyArea; }
	void detectMyArea(bool flag) { _detectMyArea = flag; }
	E_Layer layer() const { return _layer; }
	void layer(E_Layer val) { _layer = val; }
	bool fixedRotation() const { return _fixedRotation; }
	void fixedRotation(bool flag) { _fixedRotation = flag; }
	const std::vector<Area*>& overlappedAreas() const { return _overlappedAreas; }
	bool overlaps(const Area& other) const;
protected:
	Area() = delete; // 여기서 _oldTF 초기화하면 안됨. _gameObject가 아직 nullptr이기 때문에
	Area(const Area&) = delete;
	Area(GameObject* obj) : Behaviour(obj) {}
	~Area();
	virtual void awake() override;
	virtual void update() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onDestroy() override;
	virtual void onAreaEnter(Area& my, Area& other) override;
	virtual void onAreaExit(Area& my, Area& other) override;
private:
	virtual Area* createInstance(GameObject* obj) override { return new Area(obj); }
	virtual void copyTo(Component* instance) override;

	bool _detectMyArea = false;
	E_Layer _layer = E_Layer::DEFAULT;
	bool _fixedRotation = true;
	std::vector<Shape*> _shapes;
	protocol::mmo::TransformInt _oldTF;
	std::vector<Area*> _overlappedAreas;
};