#include "GameObject.h"
#include "Component.h"
#include "Utils.h"
#include "Map.h"
#include "ActionTimer.h"
#include "ObjectPool.h"
#include <stack>
using namespace protocol::mmo;


#pragma region private static
void GameObject::doRecursively(GameObject* root, const std::function<void(GameObject*)>& action, const std::function<bool(GameObject*)>& filter)
{
	static std::stack<GameObject*> stk;
	stk.push(root);
	while (!stk.empty())
	{
		GameObject* obj = stk.top();
		stk.pop();
		action(obj);
		for (GameObject* child : obj->_children)
			if (filter(child))
				stk.push(child);
	}
}
#pragma endregion




#pragma region public
ActionTimer* GameObject::addTimer(int msec, const std::function<void(GameObject*)>& action)
{
	ActionTimer* timer = ObjectPool::get<ActionTimer>();
	timer->_gameObject = this;
	timer->_delayTime = msec;
	timer->_action = action;
	_timers.push_back(timer);

	return timer;
}
void GameObject::removeTimer(ActionTimer* timer)
{
	auto it = std::find(_timers.begin(), _timers.end(), timer);
	if (it != _timers.end())
	{
		_timers.erase(it);
		ObjectPool::release<ActionTimer>(timer);
	}
}
void GameObject::parent(GameObject* val, bool evtInvoke)
{
	if (_parent == val) return;
	if (_parent != nullptr)
		_parent->_children.erase(std::find(_parent->_children.begin(), _parent->_children.end(), this));
	_parent = val;
	if (_parent != nullptr)
	{
		_parent->addChild(this);
		Utils::worldToLocal(_parent->transform(), this->transform(), _localTF);
	}
	else
	{
		_localTF.set_dir(E_Dir::BOTTOM);
		_localTF.set_y(0);
		_localTF.set_x(0);
	}

	if (_activeSelf && !_activeInHierarchy && (_parent == nullptr || _parent->_activeInHierarchy)) // 활성화
		GameObject::doRecursively(this,
			[&evtInvoke](GameObject* obj) { obj->_activeInHierarchy = true; if (evtInvoke) obj->broadcastToComponents(E_GameObjectEvent::ACTIVE, nullptr); },
			[](GameObject* obj) { return obj->_activeSelf; });
	else if (_activeInHierarchy && _parent != nullptr && !_parent->_activeInHierarchy) // 비활성화
		GameObject::doRecursively(this,
			[&evtInvoke](GameObject* obj) { obj->_activeInHierarchy = false; if (evtInvoke) obj->broadcastToComponents(E_GameObjectEvent::INACTIVE, nullptr); },
			[](GameObject* obj) { return obj->_activeSelf; });
}
void GameObject::transform(int y, int x, E_Dir dir)
{
	TransformInt* worldTF = _info.mutable_transform();
	if ((worldTF->y() == y && worldTF->x() == x && worldTF->dir() == dir))
		return;
	
	worldTF->set_dir(dir);
	worldTF->set_y(y);
	worldTF->set_x(x);
	// local 좌표 설정
	if (_parent != nullptr)
		Utils::worldToLocal(_parent->transform(), *worldTF, _localTF);

	// 자식 오브젝트의 월드좌표 설정
	if (_children.empty()) return;
	GameObject::doRecursively(this,
		[](GameObject* obj) { if (obj->_parent != nullptr) Utils::localToWorld(obj->_parent->transform(), obj->_localTF, *(obj->_info.mutable_transform())); },
		[](GameObject* obj) { return true; });
	return;
}
void GameObject::active(bool flag, bool evtInvoke)
{
	if (_activeSelf == flag) return;

	_activeSelf = flag;
	if (_activeSelf && (_parent == nullptr || (_parent->_activeInHierarchy))) // 오브젝트 활성화
		GameObject::doRecursively(this,
			[&evtInvoke](GameObject* obj)	{ obj->_activeInHierarchy = true; if (evtInvoke) obj->broadcastToComponents(E_GameObjectEvent::ACTIVE, nullptr); },
			[](GameObject* obj) { return obj->_activeSelf; });
	else if (!_activeSelf && _activeInHierarchy) // 비활성화
		GameObject::doRecursively(this,
			[&evtInvoke](GameObject* obj) { obj->_activeInHierarchy = false; if (evtInvoke) obj->broadcastToComponents(E_GameObjectEvent::INACTIVE, nullptr); },
			[](GameObject* obj) { return obj->_activeSelf; });
}
void GameObject::map(Map* val)
{
	if (_map == val) return;

	if (_parent != nullptr)
		_parent->_children.erase(std::find(_parent->_children.begin(), _parent->_children.end(), this));
	_parent = nullptr;
	this->active(false);

	// 기존 맵에서 삭제, 새로운 맵에 추가
	std::vector<GameObject*>& gameObjects = _map->_gameObjects;
	gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), this));
	_map = val;
	_map->_gameObjects.push_back(this);

	auto [baseY, baseX] = val->basePoint();
	this->active(true);
	this->transform(baseY, baseX, _info.transform().dir());
}
#pragma endregion

#pragma region protected
GameObject::GameObject(GameObject* copy)
{
	_activeSelf = copy->_activeSelf;
	_activeInHierarchy = copy->_activeInHierarchy;
	_info = copy->_info;
	_localTF = copy->_localTF;
	for (Component* component : copy->_components)
	{
		Component* newComp = component->createInstance(this);
		component->copyTo(newComp);
		newComp->invoke(E_GameObjectEvent::AWAKE, nullptr);
		_components.push_back(newComp);
	}
	for (GameObject* child : copy->_children)
	{
		GameObject* newChild = child->clone();
		newChild->_parent = this;
		_children.push_back(newChild);
	}
}
GameObject::GameObject(bool isActive, GameObject* p)
{
	_activeSelf = _activeInHierarchy = isActive;
	_info.set_id(GameObject::INVALID_ID);
	_info.set_state(E_ObjectState::NONE);
	TransformInt* worldTF = _info.mutable_transform();
	worldTF->set_dir((E_Dir)0);
	worldTF->set_y(0);
	worldTF->set_x(0);
	this->parent(p, false);
}
GameObject::GameObject(bool isActive, GameObject* copy, GameObject* p) : GameObject(copy)
{
	this->active(isActive, false);
	this->parent(p, false);
}
GameObject::~GameObject()
{
	for (Component* component : _components)
		delete component;
}
#pragma endregion

#pragma region private
void GameObject::broadcastToComponents(const E_GameObjectEvent& evt, void* arg)
{
	for (Component* component : _components)
		component->invoke(evt, arg);
	for (ActionTimer* timer : _timers)
		timer->invoke(evt, arg);
}
#pragma endregion