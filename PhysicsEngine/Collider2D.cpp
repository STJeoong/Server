#include "pch.h"
#include "Collider2D.h"
#include "World.h"

#pragma region public
void Collider2D::enabled(bool flag)
{
	if (Behaviour::enabled() == flag) return;
	Behaviour::enabled(flag);
	// broadphase에 추가or삭제
	if (flag) this->addToBroadPhase();
	else this->removeFromBroadPhase();
}
#pragma endregion

#pragma region protected
void Collider2D::onDestroy() { this->removeFromBroadPhase(); }
void Collider2D::onActiveGameObject() { this->addToBroadPhase(); }
void Collider2D::onInactiveGameObject() { this->removeFromBroadPhase(); }
void Collider2D::onAddComponent(Component* component)
{
	if (component != this) return;
	this->addToBroadPhase();
}
void Collider2D::onRemoveComponent(Component* component)
{
	if (component != this) return;
	this->removeFromBroadPhase();
}
void Collider2D::addToBroadPhase() { _key = World::addCollider(this); }
void Collider2D::removeFromBroadPhase() { World::removeCollider(_key); _key = -1; }
#pragma endregion