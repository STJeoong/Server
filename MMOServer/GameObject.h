#pragma once
#include <vector>
#include "MMO_struct.pb.h"
#include "E_GameObjectEvent.h"

using namespace protocol::mmo;
// TODO : 자식 오브젝트 만들 수 있게 할까?
class Component;
class GameObject
{
	friend class Game;
public:
	template<typename T>
	T* getComponent();
	template<typename T>
	void getComponents(std::vector<Component*>& list);
	template<typename T, typename... Args>
	T* addComponent(Args&&... args);

	// get
	// set
	GameObject* parent() const { return _parent; }
	void parent(GameObject* val);
	const std::vector<Component*>& components() const { return _components; }
	E_ObjectState state() const { return _info.state(); }
	void state(E_ObjectState val) { _info.set_state(val); }
	const TransformInt& transform() const { return _info.transform(); }
	void transform(int y, int x, E_Dir dir);
	const ObjectInfo& info() const { return _info; }
	void active(bool flag);
	bool activeSelf() const { return _activeSelf; }
	bool activeInHierarchy() const { return _activeInHierarchy; }
private:
	GameObject();
	~GameObject();
	void addChild(GameObject* child) { _children.push_back(child); }
	void broadcastToComponents(const E_GameObjectEvent& evt, void* arg);

	ObjectInfo _info;
	TransformInt _localTF;
	GameObject* _parent = nullptr;
	std::vector<GameObject*> _children;
	std::vector<Component*> _components;
	bool _activeSelf = true; // 해당 값이 true더라도 부모 오브젝트 중에 하나라도 false라면 해당 오브젝트는 활성화되지 않음.
	bool _activeInHierarchy = true; // 부모 오브젝트들과 본인 오브젝트 중 하나라도 activeSelf가 false라면 해당값은 false.
};


template<typename T>
inline T* GameObject::getComponent()
{
	T* ret = nullptr;
	for (Component* com : _components)
		if ((ret = dynamic_cast<T*>(com)) != nullptr)
			break;
	return ret;
}
template<typename T>
inline void GameObject::getComponents(std::vector<Component*>& list)
{
	T* val = nullptr;
	for (Component* com : _components)
		if ((val = dynamic_cast<T*>(com)) != nullptr)
			list.push_back(val);
}
template<typename T, typename... Args>
inline T* GameObject::addComponent(Args&&... args)
{
	Component* ret = new T(std::forward<Args>(args)...);
	ret->gameObject(this);
	_components.push_back(ret);
	return dynamic_cast<T*>(ret);
}