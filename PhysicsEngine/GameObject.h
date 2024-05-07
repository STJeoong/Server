#pragma once
#include <vector>
#include <string>
#include <queue>
#include "E_GameObjectEvent.h"
#include "Transform.h"
#include "E_Layer.h"
class Component;
// ���� ������Ʈ�� �ʱ� ��ġ�� ���� ��ǥ�� (0,0)�̰ų� ������ ���� ������Ʈ�� ���� ��ǥ�� ����.
class GameObject
{
	friend class Solver;
	friend class CollisionDetector;
	friend class World;
public:
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
	void move(const Vector2D& displacement);
	void moveTo(const Point2D& p);
	void rotate(float radian);
	void setRotation(float radian);
	const std::string& name() const { return _name; }
	std::string& name() { return _name; }
	void isActive(bool flag);
	bool isActive() const { return _isActive; }
	const E_Layer& layer() const { return _layer; }
private:
	GameObject() = default; // RigidBody�� �߰��ϸ� �߰��ǵ�������.
	GameObject& operator=(const GameObject& obj);
	~GameObject();
	void broadcast(E_GameObjectEvent evt, void* arg);
	void removeChild(GameObject* child);
	void applyReservation();
	void removeComponents();
	void processTransform(const Motion& motionInWorld);


	std::vector<Component*> _components;
	std::queue<Component*> _thingsToBeRemoved; // ���� step���� ����� component��
	GameObject* _parent = nullptr;
	std::vector<GameObject*> _children;
	Transform _worldTF; // world transform
	Transform _localTF; // local transform
	std::string _name;
	bool _isActive = true;
	bool _isRigid = false;
	E_Layer _layer = E_Layer::DEFAULT;

	// reservation ( executed next time step )
	bool _needToToggleActiveState = false;
	std::vector<Component*> _removedRigids;
	bool _needToModifyTF = false;
	Transform _arrivalTF;
};

#include "RigidBody2D.h"
template<typename T, typename... Args>
inline T* GameObject::addComponent(Args&&... args)
{
	if (typeid(T) == typeid(RigidBody2D))
	{
		if (_isRigid) return nullptr;
		_isRigid = true;
	}
	Component* ret = new T(std::forward<Args>(args)...);
	ret->setGameObject(this);
	_components.push_back(ret);
	this->broadcast(E_GameObjectEvent::ADD_COMPONENT, ret);
	return dynamic_cast<T*>(ret);
}