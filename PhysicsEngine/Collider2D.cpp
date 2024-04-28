#include "pch.h"
#include "Collider2D.h"
#include "World.h"
#include "GameObject.h"

#pragma region public
void Collider2D::enabled(bool flag)
{
	if (Behaviour::enabled() == flag) return;
	Behaviour::enabled(flag);
	if (!this->gameObject()->isActive()) return;
	if (flag) this->addToBroadPhase();
	else this->removeFromBroadPhase();
}
#pragma endregion

#pragma region protected
void Collider2D::onDestroy()
{
	if (!Behaviour::enabled() || !this->gameObject()->isActive()) return;
	this->removeFromBroadPhase();
}
void Collider2D::onActiveGameObject()
{
	if (!Behaviour::enabled()) return;
	this->addToBroadPhase();
}
void Collider2D::onInactiveGameObject()
{
	if (!Behaviour::enabled()) return;
	this->removeFromBroadPhase();
}
void Collider2D::onAddComponent(Component* component)
{
	if (component != this) return;
	if (!this->gameObject()->isActive()) return;
	this->addToBroadPhase();
}
void Collider2D::onRemoveComponent(Component* component)
{
	if (component != this) return;
	if (!this->gameObject()->isActive()) return;
	this->removeFromBroadPhase();
}
void Collider2D::addToBroadPhase() { _key = World::addCollider(this); }
void Collider2D::removeFromBroadPhase() { World::removeCollider(_key); _key = -1; }
#pragma endregion