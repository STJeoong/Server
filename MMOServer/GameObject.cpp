#include "GameObject.h"
#include "Component.h"
#include "Utils.h"
#include <stack>
using namespace protocol::mmo;

#pragma region public
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
	if (!_activeInHierarchy || (_worldTF.y() == y && _worldTF.x() == x && _worldTF.dir() == dir))
		return;
	
	_worldTF.set_y(y);
	_worldTF.set_x(x);
	_worldTF.set_dir(dir);
	// local 좌표 설정
	if (_parent != nullptr)
		Utils::worldToLocal(_parent->transform(), _worldTF, _localTF);

	// 자식 오브젝트의 월드좌표 설정
	if (_children.empty()) return;
	GameObject::doRecursively(this,
		[](GameObject* obj) { if (obj->_parent != nullptr) Utils::localToWorld(obj->_parent->transform(), obj->_localTF, obj->_worldTF); },
		[](GameObject* obj) { return true; });
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
#pragma endregion

#pragma region private
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

GameObject::GameObject(const GameObject& copy)
{
	_activeSelf = copy._activeSelf;
	_activeInHierarchy = copy._activeInHierarchy;
	_worldTF = copy._worldTF;
	_localTF = copy._localTF;
	for (Component* component : copy._components)
	{
		Component* newComp = component->createInstance();
		newComp->gameObject(this);
		component->copyTo(newComp);
		newComp->invoke(E_GameObjectEvent::AWAKE, nullptr);
		_components.push_back(newComp);
	}
	for (GameObject* child : copy._children)
	{
		GameObject* newChild = new GameObject(*child);
		newChild->_parent = this;
		_children.push_back(newChild);
	}
}
GameObject::GameObject(bool isActive, GameObject* p)
{
	_activeSelf = _activeInHierarchy = isActive;
	_worldTF.set_dir(E_Dir::BOTTOM);
	_worldTF.set_y(0);
	_worldTF.set_x(0);
	this->parent(p, false);
}
GameObject::GameObject(bool isActive, const GameObject& copy, GameObject* p) : GameObject(copy)
{
	this->active(isActive, false);
	this->parent(p, false);
}
GameObject::~GameObject()
{
	for (Component* component : _components)
		delete component;
}
void GameObject::broadcastToComponents(const E_GameObjectEvent& evt, void* arg)
{
	for (Component* component : _components)
		component->invoke(evt, arg);
}
#pragma endregion