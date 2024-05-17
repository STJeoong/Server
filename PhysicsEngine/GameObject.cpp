#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "World.h"

#pragma region public
void GameObject::removeComponent(Component* component)
{
	this->broadcast(E_GameObjectEvent::REMOVE_COMPONENT, component);

	auto it = std::find(_components.begin(), _components.end(), component);
	if (it == _components.end()) return;
	_thingsToBeRemoved.push(*it);
	_components.erase(it);
}
void GameObject::setParent(GameObject& obj)
{
	if (_parent != nullptr)
		_parent->removeChild(this);
	_parent = &obj;
	obj._children.push_back(this);
	_localTF.position(_worldTF.position() - obj._worldTF.position());
	_localTF.rotation(_worldTF.rotation() * obj._worldTF.rotation().transpose());
}
void GameObject::setChild(GameObject& obj)
{
	_children.push_back(&obj);
	if (obj._parent != nullptr)
		obj._parent->removeChild(&obj);
	obj._parent = this;
	obj._localTF.position(obj._worldTF.position() - _worldTF.position());
	obj._localTF.rotation(obj._worldTF.rotation() * _worldTF.rotation().transpose());
}
void GameObject::transform(const Motion& motionInWorld) { if (!_isActive) return; _arrivalTF += motionInWorld; _needToModifyTF = true; }
void GameObject::move(const Vector2D& displacement) { if (!_isActive) return; _arrivalTF.move(displacement); _needToModifyTF = true; }
void GameObject::moveTo(const Point2D& p) { if (!_isActive) return; _arrivalTF.position(p); _needToModifyTF = true; }
void GameObject::rotate(float radian) { if (!_isActive) return; _arrivalTF.rotate(radian); _needToModifyTF = true; }
void GameObject::setRotation(float radian) { if (!_isActive) return; _arrivalTF.rotation(Matrix22(radian)); _needToModifyTF = true; }
void GameObject::isActive(bool flag)
{
	if (_isActive == flag) return;
	if (_needToToggleActiveState) return;
	_needToToggleActiveState = true;
	if (_isActive == false)
		this->broadcast(E_GameObjectEvent::ACTIVE, nullptr);
	else
		this->broadcast(E_GameObjectEvent::INACTIVE, nullptr);
}
#pragma endregion

#pragma region private
GameObject& GameObject::operator=(const GameObject& obj)
{
	_worldTF = obj._worldTF;
	_arrivalTF = _worldTF;
	_name = obj._name;
	for (Component* otherComponent : obj._components)
	{
		Component* component = otherComponent->clone();
		_components.push_back(component);
		this->broadcast(E_GameObjectEvent::ADD_COMPONENT, component);
	}
	return *this;
}
GameObject::~GameObject()
{
	for (Component*& com : _components)
	{
		delete com;
		com = nullptr;
	}
	this->removeComponents();
	for (int i = 0; i < _children.size(); ++i)
		World::destroy(_children[i]);
}
bool GameObject::broadcast(E_GameObjectEvent evt, void* arg)
{
	bool ret = true;
	for (Component*& com : _components)
		if (!com->invoke(evt, arg))
			ret = false;
	return ret;
}
void GameObject::removeChild(GameObject* child)
{
	auto it = std::find(_children.begin(), _children.end(), child);
	_children.erase(it);
}
void GameObject::applyReservation()
{
	if (_needToModifyTF)
	{
		this->processTransform(_arrivalTF - _worldTF);
		_arrivalTF = _worldTF;
		this->broadcast(E_GameObjectEvent::MOVE_OBJECT, nullptr);
	}

	if (_needToToggleActiveState)
		_isActive = !_isActive;
	_needToModifyTF = false;
	_needToToggleActiveState = false;
	this->removeComponents();
	this->broadcast(E_GameObjectEvent::APPLY_RESERVATION, nullptr);
}
void GameObject::removeComponents()
{
	while (!_thingsToBeRemoved.empty())
	{
		Component* c = _thingsToBeRemoved.front();
		_thingsToBeRemoved.pop();
		delete c;
	}
}
void GameObject::processTransform(const Motion& motionInWorld)
{
	// 로컬 위치는 displacement.position()을 부모 오브젝트의 회전값의 반대방향으로 돌려서 더해주면 된다.
	Matrix22 rot = _parent->_worldTF.rotation().transpose();
	Vector2D localDisplacement = rot * motionInWorld.displacement();
	_localTF.move(localDisplacement);
	_localTF.rotate(motionInWorld.rotation());
	_worldTF.move(motionInWorld.displacement());
	_worldTF.rotate(motionInWorld.rotation());

	if (_children.empty())
		return;

	// for performance, stk is static. So, all gameobject share this stk.
	static std::stack<GameObject*> stk;
	while (!stk.empty())
		stk.pop();
	for (int i = 0; i < _children.size(); ++i) stk.push(_children[i]);
	while (!stk.empty())
	{
		GameObject& obj = *(stk.top());
		GameObject& parent = *(obj._parent);
		stk.pop();
		obj._worldTF.position(parent._worldTF.position() + parent._worldTF.rotation() * obj._localTF.position());
		obj._worldTF.rotation(parent._worldTF.rotation() * obj._localTF.rotation());
		for (int i = 0; i < obj._children.size(); ++i) stk.push(obj._children[i]);
	}
}
#pragma endregion