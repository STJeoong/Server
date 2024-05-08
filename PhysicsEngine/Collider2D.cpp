#include "pch.h"
#include "Collider2D.h"
#include "World.h"
#include "GameObject.h"

#pragma region public
void Collider2D::isTrigger(bool flag)
{
	if (!_needToToggleTriggerState && flag != _isTrigger)
		_needToToggleTriggerState = true;
	else if (_needToToggleTriggerState && flag == _isTrigger)
		_needToToggleTriggerState = false;
}
Point2D Collider2D::position() const
{
	const Point2D& worldObjPos = this->gameObject()->transform().position();
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	Point2D worldCenter = worldObjPos + worldObjRot * _offset;

	return worldCenter;
}
Point2D Collider2D::toLocal(const Point2D& p) const
{
	Point2D ret = p - this->position();
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	ret = worldObjRot.transpose() * ret;
	return ret;
}
Point2D Collider2D::toWorld(const Point2D& p) const
{
	Point2D ret = this->position();
	const Matrix22& worldObjRot = this->gameObject()->transform().rotation();
	ret += worldObjRot * p;
	return ret;
}
#pragma endregion

#pragma region protected
void Collider2D::onDestroy()
{
	if (!Behaviour::enabled() || !this->gameObject()->isActive()) return;
	this->removeFromWorld();
}
void Collider2D::onActiveGameObject()
{
	if (!Behaviour::enabled()) return;
	this->addToWorld();
}
void Collider2D::onInactiveGameObject()
{
	if (!Behaviour::enabled()) return;
	this->removeFromWorld();
}
bool Collider2D::onAddComponent(Component* component)
{
	if (component != this) return true;
	if (!this->gameObject()->isActive()) return true;
	this->addToWorld();
	return true;
}
void Collider2D::onRemoveComponent(Component* component)
{
	if (component != this) return;
	if (!this->gameObject()->isActive()) return;
	this->removeFromWorld();
}
void Collider2D::onEnableComponent(Component* component)
{
	if (component != this) return;
	if (!this->gameObject()->isActive()) return;
	this->addToWorld();
}
void Collider2D::onDisableComponent(Component* component)
{
	if (component != this) return;
	if (!this->gameObject()->isActive()) return;
	this->removeFromWorld();
}
void Collider2D::onApplyReservation()
{
	Behaviour::onApplyReservation();
	if (_needToToggleTriggerState) _isTrigger = !_isTrigger;
	_needToToggleTriggerState = false;
}
void Collider2D::onMove()
{
	if (!this->gameObject()->isActive() || !Behaviour::enabled() || _key == -1) return;
	World::moveCollider(_key, this->computeAABB(), { 0.0f, 0.0f });
}
void Collider2D::addToWorld() { _key = World::add(this); }
void Collider2D::removeFromWorld()
{
	if (_key == -1) return;
	World::removalReq(_key);
	_key = -1;
}
#pragma endregion