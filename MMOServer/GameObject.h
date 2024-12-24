#pragma once
#include <vector>
#include <functional>
#include "MMO_struct.pb.h"
#include "E_GameObjectEvent.h"

class ActionTimer;
class Component;
class Map;
class GameObject
{
	friend class Map;
public:
	static const int INVALID_ID = -1;
private:
	static void doRecursively(GameObject* root, const std::function<void(GameObject*)>& action, const std::function<bool(GameObject*)>& filter);




public:
	template<typename T>
	T* getComponent();
	template<typename T>
	void getComponents(std::vector<Component*>& list);
	template<typename T, typename... Args>
	T* addComponent(Args&&... args);
	ActionTimer* addTimer(int msec, const std::function<void(GameObject*)>& action);
	void removeTimer(ActionTimer* timer);

	// get
	// set
	GameObject* parent() const { return _parent; }
	void parent(GameObject* val, bool evtInvoke = true);
	const std::vector<Component*>& components() const { return _components; }
	int id() const { return _info.id(); }
	void id(int val) { _info.set_id(val); }
	protocol::mmo::E_ObjectState state() const { return _info.state(); }
	void state(protocol::mmo::E_ObjectState val) { _info.set_state(val); }
	const protocol::mmo::TransformInt& transform() const { return _info.transform(); }
	void transform(int y, int x, protocol::mmo::E_Dir dir);
	void flipX(bool val) { _info.set_flipx(val); }
	const protocol::mmo::ObjectInfo& info() const { return _info; }
	void active(bool flag, bool evtInvoke = true);
	bool activeSelf() const { return _activeSelf; }
	bool activeInHierarchy() const { return _activeInHierarchy; }
	virtual protocol::mmo::E_ObjectType objectType() const = 0;
	Map* map() const { return _map; }
	void map(Map* val);
protected:
	GameObject() = delete;
	GameObject(const GameObject&) = delete;
	GameObject(GameObject* copy);
	GameObject(bool isActvie, GameObject* p);
	GameObject(bool isActive, GameObject* copy, GameObject* p);
	virtual ~GameObject();
private:

	virtual GameObject* clone() = 0;
	void addChild(GameObject* child) { _children.push_back(child); }
	void broadcastToComponents(const E_GameObjectEvent& evt, void* arg);

	protocol::mmo::ObjectInfo _info;
	protocol::mmo::TransformInt _localTF;
	GameObject* _parent = nullptr;
	std::vector<ActionTimer*> _timers;
	std::vector<GameObject*> _children;
	std::vector<Component*> _components;
	Map* _map;
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
	Component* ret = new T(this, std::forward<Args>(args)...);
	_components.push_back(ret);
	ret->invoke(E_GameObjectEvent::AWAKE, nullptr);
	return dynamic_cast<T*>(ret);
}