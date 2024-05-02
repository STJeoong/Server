#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "RigidBody2D.h"
#include "World.h"

#pragma region public
void GameObject::removeComponent(Component* component)
{
	this->broadcast(E_GameObjectEvent::REMOVE_COMPONENT, component);
	if (typeid(*component) == typeid(RigidBody2D))
		_isRigid = false;

	auto it = std::find(_components.begin(), _components.end(), component);
	_components.erase(it);
	_thingsToBeRemoved.push(*it);
}
void GameObject::setParent(GameObject& obj)
{
	if (_parent != nullptr)
		_parent->removeChild(this);
	_parent = &obj;
	obj._children.push_back(this);
	_localTF.position() = _worldTF.position() - obj._worldTF.position();
	_localTF.rotation() = _worldTF.rotation() * obj._worldTF.rotation().transpose();
}
void GameObject::setChild(GameObject& obj)
{
	_children.push_back(&obj);
	if (obj._parent != nullptr)
		obj._parent->removeChild(&obj);
	obj._parent = this;
	obj._localTF.position() = obj._worldTF.position() - _worldTF.position();
	obj._localTF.rotation() = obj._worldTF.rotation() * _worldTF.rotation().transpose();
}
void GameObject::transform(const Motion& motionInWorld)
{
	// ���� ��ġ�� displacement.position()�� �θ� ������Ʈ�� ȸ������ �ݴ�������� ������ �����ָ� �ȴ�.
	Matrix22 rot = _parent->_worldTF.rotation().transpose();
	Vector2D localDisplacement = rot * motionInWorld.displacement();
	_localTF.move(localDisplacement);
	_localTF.rotate(motionInWorld.rotation());
	_worldTF.move(motionInWorld.displacement());
	_worldTF.rotate(motionInWorld.rotation());

	std::stack<GameObject*> stk;
	for (int i = 0; i < _children.size(); ++i) stk.push(_children[i]);
	while (!stk.empty())
	{
		GameObject& obj = *(stk.top());
		GameObject& parent = *(obj._parent);
		stk.pop();
		obj._worldTF.position() = parent._worldTF.position() + parent._worldTF.rotation() * obj._localTF.position();
		obj._worldTF.rotation() = parent._worldTF.rotation() * obj._localTF.rotation();
		for (int i = 0; i < obj._children.size(); ++i) stk.push(obj._children[i]);
	}
}
void GameObject::transform(const Vector2D& displacement) { Motion motion{ displacement,0.0f }; this->transform(motion); }
void GameObject::transform(float radian) { Motion motion{ {0.0f, 0.0f}, radian }; this->transform(motion); }
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
	_name = obj._name;
	_isRigid = obj._isRigid;
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
void GameObject::broadcast(E_GameObjectEvent evt, void* arg)
{
	for (Component*& com : _components)
		com->invoke(evt, arg);
}
void GameObject::removeChild(GameObject* child)
{
	auto it = std::find(_children.begin(), _children.end(), child);
	_children.erase(it);
}
void GameObject::update()
{
	if (_needToToggleActiveState) _isActive = !_isActive;
	_needToToggleActiveState = false;
	this->broadcast(E_GameObjectEvent::APPLY_RESERVATION, nullptr);
	this->removeComponents();
	this->broadcast(E_GameObjectEvent::UPDATE, nullptr);
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
#pragma endregion