#include "GameObject.h"
#include "Component.h"
#include "Utils.h"
#include <stack>
using namespace protocol::mmo;

#pragma region public
void GameObject::parent(GameObject* val)
{
	if (_parent != nullptr)
		_parent->_children.erase(std::find(_parent->_children.begin(), _parent->_children.end(), this));
	_parent = val;
	val->addChild(this);

	// local 좌표 설정
	Utils::worldToLocal(val->transform(), this->transform(), _localTF);
}
void GameObject::transform(int y, int x, E_Dir dir)
{
	if (!_activeInHierarchy || (_info.transform().y() == y && _info.transform().x() == x && _info.transform().dir() == dir))
		return;
	TransformInt* form = _info.mutable_transform();
	form->set_y(y);
	form->set_x(x);
	form->set_dir(dir);
	// local 좌표 설정
	if (_parent != nullptr)
		Utils::worldToLocal(_parent->transform(), *form, _localTF);

	// 자식 오브젝트의 월드좌표 설정
	if (_children.empty()) return;

	static std::stack<GameObject*> stk; // for performance, this variable is static.
	for (GameObject* child : _children)
		stk.push(child);
	while (!stk.empty())
	{
		GameObject* obj = stk.top();
		stk.pop();
		Utils::localToWorld(obj->_parent->transform(), obj->_localTF, *(obj->_info.mutable_transform()));
		for (GameObject* child : obj->_children)
			stk.push(child);
	}
}
void GameObject::active(bool flag)
{
	if (_activeSelf == flag) return;

	_activeSelf = flag;
	static std::stack<GameObject*> stk; // for performance, this variable is static.
	if (_activeSelf && (_parent == nullptr || (_parent->_activeInHierarchy))) // 오브젝트 활성화
	{
		// 자식 오브젝트들 중에서 _activeSelf이면 stk.push
		_activeInHierarchy = true;
		this->broadcastToComponents(E_GameObjectEvent::ACTIVE, nullptr);
		for (GameObject* child : _children)
			if (child->_activeSelf)
				stk.push(child);
		while (!stk.empty())
		{
			GameObject* obj = stk.top();
			stk.pop();
			obj->_activeInHierarchy = true;
			obj->broadcastToComponents(E_GameObjectEvent::ACTIVE, nullptr);
			for (GameObject* child : obj->_children)
				if (child->_activeSelf)
					stk.push(child);
		}
	}
	else if (!_activeSelf && _activeInHierarchy) // 비활성화
	{
		_activeInHierarchy = false;
		this->broadcastToComponents(E_GameObjectEvent::INACTIVE, nullptr);
		for (GameObject* child : _children)
			if (child->_activeSelf)
				stk.push(child);
		while (!stk.empty())
		{
			GameObject* obj = stk.top();
			stk.pop();
			obj->_activeInHierarchy = false;
			obj->broadcastToComponents(E_GameObjectEvent::INACTIVE, nullptr);
			for (GameObject* child : obj->_children)
				if (child->_activeSelf)
					stk.push(child);
		}
	}
}
#pragma endregion

#pragma region private
GameObject::GameObject()
{
	_info.set_id(0);
	_info.set_state(E_ObjectState::NONE);
	TransformInt* form = _info.mutable_transform();
	form->set_dir(E_Dir::BOTTOM);
	form->set_y(0);
	form->set_x(0);
	_localTF.set_dir(E_Dir::BOTTOM);
	_localTF.set_y(0);
	_localTF.set_x(0);
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