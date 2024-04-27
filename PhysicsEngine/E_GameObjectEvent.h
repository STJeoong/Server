#pragma once

enum class E_GameObjectEvent
{
	//GameObject Event
	ACTIVE,
	INACTIVE,
	UPDATE,
	DESTROY,

	// Physics
	COLLISION_ENTER,
	COLLISION_STAY,
	COLLISION_EXIT,
	TRIGGER_ENTER,
	TRIGGER_STAY,
	TRIGGER_EXIT,



	// Inner component
	// Add or remove component
	ADD_COMPONENT,
	REMOVE_COMPONENT,

	// change RigidBody Option ( 형태가 변한경우, 밀도가 변한 경우 ) <= 형태가 변했으면 broad phase도 변경해야됨. 그게아니면 massdata만 변경하면됨.
	CHANGE_COLLIDER_VALUE,

	// enable or disable component
	ENABLE_COMPONENT,
	DISABLE_COMPONENT,
};