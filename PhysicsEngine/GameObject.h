#pragma once
#include <vector>
#include <string>
#include "E_GameObjectEvent.h"
#include "Transform.h"
class Component;
class GameObject
{
public:
	static GameObject* find(const std::string& name);
	static GameObject* instantiate(GameObject* obj = nullptr, GameObject* parent = nullptr, bool active = true);
	static void destroy(GameObject*& obj);

	template<typename T>
	T* getComponent() { T* ret = nullptr; for (Component* com : _components) if ((ret = dynamic_cast<T*>(com)) != nullptr) break; return ret; }
	template<typename T, typename... Args>
	T* addComponent(Args&&... args);
	// you have to set component's pointer to nullptr. because this pointer becomes orphan.
	void removeComponent(Component* component);
	void setParent(GameObject& obj);
	void setChild(GameObject& obj);
	const std::vector<Component*>& components() const { return _components; }
	const Transform& transform() const { return _worldTF; }
	void transform(const Motion& motionInWorld);
	void transform(const Vector2D& displacement);
	void transform(float radian);
	const std::string& name() const { return _name; }
	std::string& name() { return _name; }
	void isActive(bool flag);
	bool isActive() const { return _isActive; }
private:
	GameObject() = default; // RigidBody는 추가하면 추가되도록하자.
	GameObject(const GameObject& obj);
	~GameObject();
	void broadcast(E_GameObjectEvent evt, void* arg);
	void removeChild(GameObject* child);

	static std::vector<GameObject*> s_gameObjects;
	static GameObject* s_root;

	std::vector<Component*> _components;
	GameObject* _parent = nullptr;
	std::vector<GameObject*> _children;
	Transform _worldTF; // world transform
	Transform _localTF; // local transform
	std::string _name;
	bool _isActive = true;
	bool _isRigid = false;
};

#include "RigidBody2D.h"
template<typename T, typename... Args>
inline T* GameObject::addComponent(Args&&... args)
{
	Component* ret = new T(std::forward<Args>(args)...);
	RigidBody2D* rigid = dynamic_cast<RigidBody2D*>(ret);
	if (_isRigid && rigid != nullptr)
	{
		delete ret;
		return nullptr;
	}
	else if (!_isRigid && rigid != nullptr)
		_isRigid = true;
	ret->setGameObject(this);
	_components.push_back(ret);
	for (Component* com : _components)
		com->invoke(E_GameObjectEvent::ADD_COMPONENT, ret);
	return dynamic_cast<T*>(ret);
}