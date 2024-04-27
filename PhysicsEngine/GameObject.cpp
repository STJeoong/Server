#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "RigidBody2D.h"

std::vector<GameObject*> GameObject::s_gameObjects;
#pragma region public
GameObject* GameObject::find(const std::string& name)
{
	for (GameObject*& obj : s_gameObjects)
		if (obj->_name == name)
			return obj;
	return nullptr;
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
	_parent = &obj;
	obj._children.push_back(this);
}
void GameObject::setChild(GameObject& obj)
{
	_children.push_back(&obj);
	obj._parent = this;
}
void GameObject::transform(const Transform& displacement)
{
	// TODO : 자식 오브젝트 모두 이동

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
}
void GameObject::broadcast(E_GameObjectEvent evt, void* arg)
{
	for (Component*& com : _components)
		com->invoke(evt, arg);
}
#pragma endregion

