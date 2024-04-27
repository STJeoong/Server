#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "RigidBody2D.h"


#pragma region public
GameObject* GameObject::find(const std::string& name)
{
	for (GameObject*& obj : s_gameObjects)
		if (obj->_name == name)
			return obj;
	return nullptr;
}
GameObject* GameObject::instantiate(GameObject* obj, GameObject* parent, bool active)
{
	if (s_root == nullptr) s_root = new GameObject();
	GameObject* ret;
	if (obj == nullptr) ret = new GameObject();
	else ret = new GameObject(*obj);
	ret->_isActive = active;
	if (parent == nullptr) ret->_parent = s_root;
	else ret->_parent = parent;
	s_root->_children.push_back(ret);
	s_gameObjects.push_back(ret);
	return ret;
}
void GameObject::destroy(GameObject*& obj)
{
	for (int i = 0; i < s_gameObjects.size(); ++i)
		if (s_gameObjects[i] == obj)
		{
			s_gameObjects.erase(s_gameObjects.begin() + i);
			break;
		}
	obj->broadcast(E_GameObjectEvent::DESTROY, nullptr);
	delete obj;
	obj = nullptr;
}
void GameObject::removeComponent(Component* component)
{
	for (Component* com : _components)
		com->invoke(E_GameObjectEvent::REMOVE_COMPONENT, component);
	
	for (int i = 0; i < _components.size(); ++i)
		if (_components[i] == component)
		{
			_components.erase(_components.begin() + i);
			break;
		}
	delete component;
}
void GameObject::setParent(GameObject& obj)
{
	_parent->removeChild(this);
	_parent = &obj;
	obj._children.push_back(this);
	_localTF.position() = _worldTF.position() - obj._worldTF.position();
	_localTF.rotation() = _worldTF.rotation() * obj._worldTF.rotation().transpose();
}
void GameObject::setChild(GameObject& obj)
{
	_children.push_back(&obj);
	obj._parent->removeChild(&obj);
	obj._parent = this;
	obj._localTF.position() = obj._worldTF.position() - _worldTF.position();
	obj._localTF.rotation() = obj._worldTF.rotation() * _worldTF.rotation().transpose();
}
void GameObject::transform(const Motion& motionInWorld)
{
	// 로컬 위치는 displacement.position()을 부모 오브젝트의 회전값의 반대방향으로 돌려서 더해주면 된다.
	Matrix22 rot(_parent->_localTF.rotation().radian() * -1);
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
void GameObject::isActive(bool flag)
{
	if (_isActive == flag)
		return;
	_isActive = flag;
	if (_isActive == false)
		this->broadcast(E_GameObjectEvent::ACTIVE, nullptr);
	else
		this->broadcast(E_GameObjectEvent::INACTIVE, nullptr);
}
#pragma endregion

#pragma region private
GameObject::GameObject(const GameObject& obj) : _worldTF(obj._worldTF), _name(obj._name)
{
	for (Component* otherComponent : obj._components)
	{
		Component* component = otherComponent->clone();
		for (Component* myComponent : _components)
			myComponent->invoke(E_GameObjectEvent::ADD_COMPONENT, component);
		_components.push_back(component);
	}
}
GameObject::~GameObject()
{
	for (Component*& com : _components)
	{
		delete com;
		com = nullptr;
	}
	for (int i = 0; i < _children.size(); ++i)
		GameObject::destroy(_children[i]);
}
void GameObject::broadcast(E_GameObjectEvent evt, void* arg)
{
	for (Component*& com : _components)
		com->invoke(evt, arg);
}
void GameObject::removeChild(GameObject* child)
{
	for (int i = 0; i < _children.size(); ++i)
		if (_children[i] == child)
		{
			_children.erase(_children.begin() + i);
			break;
		}
}
#pragma endregion

std::vector<GameObject*> GameObject::s_gameObjects;
GameObject* GameObject::s_root = nullptr;