#pragma once

enum class E_GameObjectEvent
{
	NONE,

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

	// change RigidBody Option ( ���°� ���Ѱ��, �е��� ���� ��� ) <= ���°� �������� broad phase�� �����ؾߵ�. �װԾƴϸ� massdata�� �����ϸ��.
	CHANGE_COLLIDER_VALUE,

	// enable or disable component
	ENABLE_COMPONENT,
	DISABLE_COMPONENT,

	// ���� step���� World�� ��û�� �͵� ���� ( RigidBody type ����, GameObject active ���� ����, Collider Trigger ���� ���� )
	APPLY_RESERVATION,
};